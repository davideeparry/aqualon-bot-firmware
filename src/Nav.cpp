#include "Nav.h"
#include <Metro.h>

void Nav::init() {

    LogInfo("NAV Init");
    // #if defined(SIMULATION) || defined(LABBUILD)
    #if 1
        // Load simulator coordinates
        // Trout Lake North
        /*
        waypoints.add(49.257182,-123.062937);
        waypoints.add(49.257042,-123.062331);
        waypoints.add(49.256752,-123.062331);
        waypoints.add(49.256907,-123.062930);
        */
        // Trout Lake short path
        /*
        waypoints.add(49.257192,-123.062945);
        waypoints.add(49.257193,-123.062686);
        waypoints.add(49.257062,-123.062804);
        waypoints.add(49.257064,-123.063068);
        */
        // Trout Lake North to South
        /*
        waypoints.add(49.257185,-123.062932);
        waypoints.add(49.256627,-123.063015);
        waypoints.add(49.255988,-123.062730);
        waypoints.add(49.255630,-123.062317);
        waypoints.add(49.255049,-123.061638);
        waypoints.add(49.254868,-123.061605);
        waypoints.add(49.254665,-123.061807);
        waypoints.add(49.254379,-123.061705);
        */
        // AQ Pond
        waypoints.add(49.279003,-122.917036);
        waypoints.add(49.279065,-122.916997);
        waypoints.add(49.279279,-122.916931);
        waypoints.add(49.279272,-122.916784);
        waypoints.add(49.279229,-122.916828);
        waypoints.add(49.279146,-122.916821);
        waypoints.add(49.279006,-122.916880);
    #else
    #endif

    setNavState(NAV_STATE_STARTUP);
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
                float forward = 0.7;
                Motors::instance().setLeft(forward);
                Motors::instance().setRight(forward);
                discoveryStartTime = millis();
                gps->markOld();
                setNavState(NAV_STATE_DISCOVERY);
            }
            break;
        }
        case NAV_STATE_DISCOVERY:
        {
            if(millis() - discoveryStartTime > DISCOVERY_TIME_MS && gps->isNew() && 
                mpu->hasData() && gps->getSpeed() > GPS_MIN_SPEED) {
                initVectors();
                initErr();
            }
            break;
        }
        case NAV_STATE_RUN:
        {
            updateVectors(timeDeltaMS);
            updateErr();

            // Check if waypoint is reached
            if(errDist < NAV_DISTANCE_ARRIVAL)
            {
                setTargetNext();
                break;
            }

            // Calculate new motor commands
            static const float anglePtoDClampValue = 2.0;
            static const float anglePtoCClampValue = 0.8;
            static const float distPtoCClampValue = 6; // Start slowing when this far from target

            static const float targetCommon = 0.7;
            static const float maxDiff = 0.7;
            static const float anglePtoCCoeff = targetCommon / anglePtoCClampValue;
            static const float distPtoCCoeff = targetCommon / distPtoCClampValue;
            static const float anglePtoDCoeff = maxDiff / anglePtoDClampValue;
            static const float angleDtoDCoeff = -2.0;

            float angleP = errAngle;
            float anglePtoDClamp = min(angleP, anglePtoDClampValue);
            float anglePtoCClamp = min(angleP, anglePtoCClampValue);
            // float angleD = velocity.getZ();
            float angleD = (errAngle - lastErrAngle) / (timeDeltaMS / 1000.0);
            float distP = errDist;
            float distPtoCClamp = min(errDist, distPtoCClampValue);
            float distD = Gps::instance().getSpeed();

            // Differential term
            float motorDiff = (anglePtoDClamp * anglePtoDCoeff) - (angleD * angleDtoDCoeff);

            // Common term
            float motorComm = (distPtoCClamp * distPtoCCoeff) - (anglePtoCCoeff * anglePtoCClamp);

            if(debugLog) LogDebug("ErrAP: %4.2f, ErrAD: %4.2f, ErrDP: %4.2f", angleP, angleD, distP)
            Motors::instance().setDiffCommon(motorDiff, motorComm);
            break;
        }
        case NAV_STATE_MANUAL:
        {
            updateVectors(timeDeltaMS);
            break;
        }
        case NAV_STATE_HALT:
        {
            updateVectors(timeDeltaMS);
            Motors::instance().setLeft(0);
            Motors::instance().setRight(0);
            break;
        }
        default:
            LogError("Navigation is in an unknown state. Halting");
            setNavState(NAV_STATE_HALT);
            break;
    }
}

void Nav::initVectors() {
    // Discovery time has expired, we should have a valid heading now
    LogInfo("Discovery time up. Initializing Vectors.");
    double angle = gps->getCourseRadians();
    position = Point(gps->getLat(), gps->getLon()).getVec3d(angle);
    velocity = Vec3d(gps->getSpeedX(), gps->getSpeedY(), mpu->getGZ());
    // int index = setTargetNearest();
    LogInfo("Waypoints:");
    waypoints.print();
    int index = setTarget(1);
    if(index >= 0) {
        LogInfo("Starting waypoint: %d", index);
        setNavState(NAV_STATE_RUN);
    } else {
        LogError("Could not find starting waypoint. Halting");
        setNavState(NAV_STATE_HALT);
    }
    gps->markOld();
}

void Nav::initErr() {
    updateErr();
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
        // Vec3d position_gps = Point(gps->getLat(), gps->getLon()).getVec3d(gps->getCourseRadians());
        float x = (gps_pos.getX() * GPS_POS_WEIGHT) + (position.getX() * (1.0 - GPS_POS_WEIGHT));
        float y = (gps_pos.getY() * GPS_POS_WEIGHT) + (position.getY() * (1.0 - GPS_POS_WEIGHT));
        // float z = (gps_course * GPS_COURSE_WEIGHT) + (position.getZ() * (1.0 - GPS_COURSE_WEIGHT));
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
            // Vec3d position_gps = Point(gps->getLat(), gps->getLon()).getVec3d(position.getZ());
            position = Vec3d(x, y, position.getZ());
        }
    }
}

void Nav::updateErr() {
    // Update destination errors
    Point target = getTarget();
    if(target) {
        lastErrAngle = errAngle;
        // Figure angle and distance to nearest waypoint
        Point pos = Point(position);
        Point toTarget = target - pos;
        lastErrDist = errDist;
        errDist = toTarget.getDist();
        float targetAngle = toTarget.getAngle();

        // Get closest distance from current heading to target angle
        errAngle = targetAngle - position.getZ();
        if(abs(errAngle) > PI) {
            if(errAngle < 0) {
                errAngle = 2 * PI + errAngle;
            } else {
                errAngle = 2 * PI - errAngle;
            }
        }
    } else {
        LogError("Nav - No target set (target negative)");
    }
}

Point Nav::getTarget() {
    if(targetWaypoint >= 0) {
        return waypoints.get(targetWaypoint);
    } else {
        return target;
    }
}

void Nav::setTarget(Point p) {
    target = p;
    targetWaypoint = -1;
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

int Nav::setTargetNearest() {
    Point pos(position);
    int index = waypoints.getNearest(pos);
    if(index >= 0) {
        targetWaypoint = index;
    }
    return index;
}

int Nav::setTargetNext() {
    if(waypoints.length() > 0) {
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
        
        case NAV_STATE_DISCOVERY:
            statusLed.Blink(250,750).Forever();
            break;
    
        case NAV_STATE_RUN:
            statusLed.Blink(125,1875).Forever();
            break;
        
        case NAV_STATE_HALT:
            statusLed.Breathe(2000).Forever();
            break;
        
        case NAV_STATE_MANUAL:
            statusLed.Blink(500,500).Forever();
            break;
    
        default:
            break;
    }
}

// Waypoints functions

int WaypointList::add(double lat, double lon) {
    Point newP(lat, lon);
    return add(newP);
}

int WaypointList::add(Point wp) {
    if(size < MAX_WAYPOINTS) {
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
        double dist = (thisWP - target).getDist();
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
    for(size_t i=0; i<10; i++) {
        Point wp = waypoints[i];
        LogInfo("WP %d, x:%4.2f, y:%4.2f, lat:%8.6f, lon:%8.6f", i, wp.getX(), wp.getY(), wp.getLat(), wp.getLon());
    }
}