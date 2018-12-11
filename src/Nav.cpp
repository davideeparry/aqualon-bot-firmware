#include "Nav.h"
#include "Paths.h"
#include <Metro.h>

void Nav::init() {

    LogInfo("NAV Init");
    // #if defined(SIMULATION) || defined(LABBUILD)
    #if 1
    loadPathsAQ();
    #endif

    setNavState(NAV_STATE_STARTUP);
}

// User functions
int Nav::start() {
    
    switch (navState)
    {
        case NAV_STATE_STARTUP:
            return 1;
        
        case NAV_STATE_WAIT:
            setNavState(NAV_STATE_DISCOVERY);
            return 0;

        case NAV_STATE_DISCOVERY:
            return 0;

        case NAV_STATE_RUN:
            return 0;

        case NAV_STATE_STAY:
            int i = setTargetNearest();
            if(-1 == i) {
                return 1;
            } else {
                setNavState(NAV_STATE_RUN);
            }
    
        default:
            break;
    }
}

int Nav::stop() {
    setNavState(NAV_STATE_WAIT);
    return 0;
}

void Nav::update() {
    statusLed.Update();
    if(1 != timer.check()) return;

    debugLog = debugTimer.check();

    int timeNow = millis();
    int timeDeltaMS = timeNow - lastUpdateTime;
    lastUpdateTime = timeNow;

    
    switch (navState)
    {
        case NAV_STATE_STARTUP: 
        {
            // Wait until we have first GPS reading
            if(gps->isNew()) {
                LogInfo("Nav: got GPS, switching to discovery");
                LogInfo("Lat: %f", gps->getLat());
                LogInfo("Lon: %f", gps->getLon());
                float forward = 1.0;
                Motors::instance().setDiffCommon(0, forward);
                discoveryStartTime = millis();
                gps->markOld();
                setNavState(NAV_STATE_WAIT);
            }
            break;
        }
        case NAV_STATE_WAIT:
        {
            // Wait for start command
            if(lastGpsTime > -1) {
                updateVectors(timeDeltaMS);
            }
            Motors::instance().setLeft(0);
            Motors::instance().setRight(0);
            break;
        }
        case NAV_STATE_DISCOVERY:
        {
            if(millis() - discoveryStartTime > DISCOVERY_TIME_MS && gps->isNew() && mpu->hasData()) {
                initVectors();
                initErr();
            }
            break;
        }
        case NAV_STATE_RUN:
        {
            // Run follows the current path. If no waypoints are loaded, 
            updateVectors(timeDeltaMS);
            updateErr(getTarget());
            updateDrive(timeDeltaMS);

            // Check if waypoint is reached
            if(errDist < NAV_DISTANCE_ARRIVAL)
            {
                int i = setTargetNext();
                if(i < 0 || waypoints.length() == 1) {
                    setNavState(NAV_STATE_STAY);
                }
            }
            break;
        }
        case NAV_STATE_MANUAL:
        {
            updateVectors(timeDeltaMS);
            break;
        }
        case NAV_STATE_STAY:
        {
            // Stay will circle the currently loaded target point
            updateVectors(timeDeltaMS);
            updateErr(getCircleTarget());
            updateDrive(timeDeltaMS);
            break;
        }
        default:
            LogError("Navigation is in an unknown state. Halting");
            setNavState(NAV_STATE_WAIT);
            break;
    }
}

void Nav::initVectors() {
    // Discovery time has expired, we should have a valid heading now
    LogInfo("Discovery time up. Initializing Vectors.");
    double angle = gps->getCourseRadians();
    position = Point(gps->getLat(), gps->getLon()).getVec3d(angle);
    velocity = Vec3d(gps->getSpeedX(), gps->getSpeedY(), mpu->getGZ());
    LogInfo("Waypoints:");
    waypoints.print();

    // Set target to first waypoint
    int index = setTarget(0);
    if(index >= 0) {
        LogInfo("Starting waypoint: %d", index);
        setNavState(NAV_STATE_RUN);
    } else {
        LogError("Could not find starting waypoint. Halting");
        setTarget(getStayTarget());
        setNavState(NAV_STATE_STAY);
    }
    gps->markOld();
}

void Nav::initErr() {
    updateErr(getTarget());
    lastErrAngle = errAngle;
}

void Nav::updateVectors(int timeDeltaMS) {
    // Estimate new position and velocity
    Gps* gps = &Gps::instance();
    Mpu* mpu = &Mpu::instance();

    Vec3d last_position = position;
    Vec3d last_velocity = velocity;

    // Update position based on last velocity
    position = last_position + (last_velocity * ((float)timeDeltaMS / 1000.0));

    // Get MPU acceleration (in boat frame of reference)
    Vec3d frameAccel = Vec3d(mpu->getAX(), mpu->getAY(), 0);

    // Rotate to get acceleration in global frame
    acceleration = rotate(frameAccel, position.getZ());

    // TODO: include acceleration in velocity update (maybe)
    velocity = Vec3d(gps->getSpeedX(), gps->getSpeedY(), mpu->getGZ());

    // Add weighted GPS term if new gps is available
    if(gps->isNew()) {
        gps->markOld();
        Point gps_pos = Point(gps->getLat(), gps->getLon());
        double gps_course = gps->getCourseRadians();
        float x = (gps_pos.getX() * GPS_POS_WEIGHT) + (position.getX() * (1.0 - GPS_POS_WEIGHT));
        float y = (gps_pos.getY() * GPS_POS_WEIGHT) + (position.getY() * (1.0 - GPS_POS_WEIGHT));

        // Average angles using arctan
        float pos_z = position.getZ();
        float z = atan2((sin(pos_z) + sin(gps_course)), (cos(pos_z) + cos(gps_course)));
        z = fmod(z + 2.0*PI, 2.0*PI);
        if(gps->getSpeed() > GPS_MIN_SPEED && velocity.getZ() < GPS_MAX_GYRO) {
            // Only use gps heading if we're going fast enough and rotating slow enough
            LogDebug("Nav - GPS term with course: %f & %f -> %f", gps_course, pos_z, z);
            position = Vec3d(x, y, z);
        } else {
            LogDebug("Nav - GPS term NO course");
            position = Vec3d(x, y, position.getZ());
        }
    }
}

void Nav::updateErr(Point target) {
    // Update destination errors
    if(target) {
        lastErrAngle = errAngle;
        // Figure angle and distance to nearest waypoint
        Point pos = Point(position);
        lastErrDist = errDist;
        errDist = getDist(target, pos);
        float targetAngle = getAngle(target, pos);

        // Get closest distance from current heading to target angle
        errAngle = targetAngle - position.getZ();
        if(abs(errAngle) > PI) {
            if(errAngle < 0) {
                errAngle = 2 * PI + errAngle;
            } else {
                errAngle = 2 * PI - errAngle;
            }
        }
        // LogInfo("Target lat: %6.4f, lon: %6.4f, x: %4.2f, y: %4.2f", 
        //         target.getLat(), target.getLon(), target.getX(), target.getY());
        // LogInfo("Boat:  lat: %6.4f, lon: %6.4f, x: %4.2f, y: %4.2f", 
        //         pos.getLat(), pos.getLon(), pos.getX(), pos.getY());
        // LogInfo("Diff:  lat: %6.4f, lon: %6.4f, x: %4.2f, y: %4.2f", 
        //         pos.getLat(), pos.getLon(), pos.getX(), pos.getY());
        // LogInfo("errA: %4.2f, errD: %4.2f", errAngle, errDist);
    } else {
        LogError("Nav - No target set (target negative)");
    }
}

void Nav::updateDrive(int timeDeltaMS) {
    // Calculate new motor commands
    static const float anglePtoDClampValue = 2.0;
    static const float anglePtoCClampValue = 1.2;
    static const float distPtoCClampValue = 6; // Start slowing when this far from target

    static const float targetCommon = 0.8;
    static const float maxDiff = 0.7;
    static const float anglePtoCCoeff = targetCommon / anglePtoCClampValue;
    static const float distPtoCCoeff = targetCommon / distPtoCClampValue;
    static const float anglePtoDCoeff = maxDiff / anglePtoDClampValue;
    static const float angleDtoDCoeff = 2.0;
    static const float angleDClampValue = maxDiff / angleDtoDCoeff;

    float angleP = errAngle;
    float anglePtoDClamp = max(min(angleP, anglePtoDClampValue), -anglePtoDClampValue);
    float anglePtoCClamp = min(abs(angleP), anglePtoCClampValue);
    // float angleD = velocity.getZ();
    float angleD = (errAngle - lastErrAngle) / (timeDeltaMS / 1000.0);
    angleD = max(min(angleD, angleDClampValue), -angleDClampValue);
    float distP = errDist;
    float distPtoCClamp = min(errDist, distPtoCClampValue);
    float distD = Gps::instance().getSpeed();

    // Differential term
    float motorDiff = (anglePtoDClamp * anglePtoDCoeff) + (angleD * angleDtoDCoeff);

    // Common term
    float motorComm = (distPtoCClamp * distPtoCCoeff) - (anglePtoCCoeff * anglePtoCClamp);
    // if(debugLog) LogInfo("ErrAP:      %4.2f, ErrAD:      %4.2f, ErrDP:      %4.2f", angleP, angleD, distP);
    // if(debugLog) LogInfo("ErrACClamp: %4.2f, ErrADClamp: %4.2f, ErrDCClamp: %4.2f", anglePtoCClamp, anglePtoDClamp, distPtoCClamp);
    // if(debugLog) LogInfo("MotorD:     %4.2f, MotorC:     %4.2f", motorDiff, motorComm);
    Motors::instance().setDiffCommon(motorDiff, motorComm);
}

Point Nav::getTarget() {
    if(targetWaypoint >= 0) {
        return waypoints.get(targetWaypoint);
    } else {
        return target;
    }
}

// Get a new target to the right of current position to begin circling
Point Nav::getStayTarget() {
    float z = position.getZ();
    float x = position.getX() + NAV_STAY_RADIUS * cos(z + PI/2.0);
    float y = position.getY() + NAV_STAY_RADIUS * sin(z + PI/2.0);
    LogDebug("Generating stay target: %4.2f, %4.2f", x, y);
    return Point(Vec3d(x, y, 0));
}

// Get a point just ahead of our position in a circle around the target
Point Nav::getCircleTarget() {
    static Metro tm(500);
    Point target = getTarget();
    Point pos = Point(position);
    float angleFromTarget = getAngle(pos, target);
    float angleAhead = angleFromTarget + 1.0;
    Point circleTarget = Point(Vec3d(target.getX() + NAV_STAY_RADIUS * cos(angleAhead), 
                               target.getY() + NAV_STAY_RADIUS * sin(angleAhead), 0));
    if(1 == tm.check()) {
        LogDebug("Target (%4.2f,%4.2f), CircleTarget: (%4.2f,%4.2f)", target.getX(), target.getY(), 
                  circleTarget.getX(), circleTarget.getY());
    }
    return circleTarget;
}

int Nav::setTarget(Point p) {
    target = p;
    targetWaypoint = -1;
    return 0;
}

int Nav::setTarget(unsigned index) {
    if(index < waypoints.length()) {
        targetWaypoint = index;
        return index;
    }
    else {
        return -1;
    }
}

int Nav::setTarget(double lat, double lon) {
    return setTarget(Point(lat, lon));
}

void Nav::clearWaypoints() {
    if(-1 != targetWaypoint) {
        setTarget(waypoints.get(targetWaypoint));
    }
    waypoints.clear();
    setNavState(NAV_STATE_STAY);
}

int Nav::setTargetNearest() {
    Point pos(position);
    int index = waypoints.getNearest(pos);
    if(index >= 0) {
        targetWaypoint = index;
    }
    return index;
}

int Nav::setTargetNext() {
    // Check if we are on waypoints now
    if(targetWaypoint >= 0 && waypoints.length() > 0) {
        targetWaypoint = (targetWaypoint + 1) % waypoints.length();
        LogInfo("Setting waypoint %d", targetWaypoint);
        return targetWaypoint;
    }
    return -1;
}

void Nav::setNavState(NavigationState state) {
    
    navState = state;
    switch (state)
    {
        case NAV_STATE_STARTUP:
            statusLed.Blink(125,125).Forever();
            break;

        case NAV_STATE_WAIT:
            statusLed.Blink(125, 625);
            break;
        
        case NAV_STATE_DISCOVERY:
            statusLed.Blink(250,750).Forever();
            break;
    
        case NAV_STATE_RUN:
            statusLed.Blink(125,1875).Forever();
            break;
        
        case NAV_STATE_MANUAL:
            statusLed.Blink(500,500).Forever();
            break;

        case NAV_STATE_STAY:
            statusLed.Blink(125,1875).Forever();
            break;
    
        default:
            break;
    }
}

// Save current state, including waypoints, to persistent disk file
int Nav::saveState() {
    File dbfile = SD.open(NAV_PERSISTENT_FILE_NAME, FILE_WRITE); // Should truncate by default
    Nav* nav = &Nav::instance();
    size_t navSize = sizeof(*nav);
    LogDebug("Writing Nav state to disk, %d bytes", navSize);
    size_t w = dbfile.write((uint8_t*)(nav), sizeof(*nav));
    if(w != navSize) {
        LogError("Failed to write Nav state to disk. Wrote %d bytes.", w);
        return 1;
    }
    return 0;
}

int Nav::restoreState() {
    return 0;
}

// Waypoints functions

int WaypointList::add(double lat, double lon) {
    Point newP(lat, lon);
    return add(newP);
}

int WaypointList::add(Point wp) {
    if(size < MAX_WAYPOINTS) {
        // Check for existing, nearby waypoints
        for(size_t i=0; i<size; i++) {
            float distance = getDist(wp, waypoints[i]);
            if(distance < WAYPOINT_MIN_DIST) return -1;
        }
        waypoints[size] = wp;
        size++;
        LogInfo("New waypoint");
        LogInfo("Lat: %f", wp.getLat());
        LogInfo("Lon: %f", wp.getLon());
        return size - 1;
    } else {
        LogError("Max waypoints filled");
        return -1;
    }
}

int WaypointList::remove(size_t index) {
    if(size <= index) return -1;
    size_t removed = index;
    while(index < size) {
        waypoints[index] = waypoints[index+1];
        index++;
    }
    size--;
    return removed;
}

int WaypointList::remove(double lat, double lon) {
    return remove(Point(lat, lon));
}

int WaypointList::remove(Point wp) {
    for(size_t i=0; i<size; i++) {
        float distance = getDist(wp, waypoints[i]);
        if(distance < WAYPOINT_MIN_DIST) {
            return remove(waypoints[i]);
        }
    }
    return -1;
}

void WaypointList::clear() {
    size = 0;
}

Point WaypointList::get(unsigned index) {
    if(index >= 0 && index < size && size > 0) {
        return waypoints[index];
    }
    return Point();
}

int WaypointList::getNearest(Point target) {
    if(size == 0) {
        LogError("No waypoints loaded");
        return -1;
    }
    int closest = -1;
    double closestDist = 100000.0;
    Point thisWP;
    for(int i = 0; i < (int)size; i++) {
        thisWP = waypoints[i];
        double dist = getDist(thisWP, target);
        LogInfo("wp distance: %f", dist);
        if(dist < closestDist) {
            closestDist = dist;
            closest = i;
        }
    }
    return closest;
}

size_t WaypointList::length() {
    return size;
}

void WaypointList::print() {
    char printbuff[128];
    for(size_t i=0; i<size; i++) {
        Point wp = waypoints[i];
        LogInfo("WP %d, x:%4.2f, y:%4.2f, lat:%8.6f, lon:%8.6f", i, wp.getX(), wp.getY(), wp.getLat(), wp.getLon());
    }
}