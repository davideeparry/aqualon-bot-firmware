#include "Nav.h"
#include <Metro.h>

void Nav::init() {

    LOG("NAV Init")
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
        // AQ Pond
        waypoints.add(49.279003,-122.917036);
        waypoints.add(49.279279,-122.916931);
        waypoints.add(49.279272,-122.916784);
        waypoints.add(49.279126,-122.916831);
    #else
    #endif

    navState = NAV_STATE_STARTUP;
}

void Nav::update() {
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
                LOG("Nav: got GPS, switching to discovery");
                LOGV("Lat: ", gps->getLat());
                LOGV("Lon: ", gps->getLon());
                int forward = MOTOR_MAX / 2;
                Motors::instance().setLeft(forward);
                Motors::instance().setRight(forward);
                discoveryStartTime = millis();
                gps->markOld();
                navState = NAV_STATE_DISCOVERY;
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
            float angleP = errAngle;
            float angleD = velocity.getZ();
            float distP = min(errDist, distClamp);
            float distD = Gps::instance().getSpeed();

            // Differential term
            float motorDiff = ((angleP * angleToDiffP) + (angleD * angleToDiffD) + 
                              (distP * distToDiffP) + (distD * distToDiffD)) * diffGain;
            if(debugLog) LOGV("New motor diff:", motorDiff);

            // Common term
            float anglePComm = min(abs(angleP), angleClamp);
            float motorComm = ((anglePComm * angleToCommP) + (abs(angleD) * angleToCommD) + 
                              (distP * distToCommP) + (distD * distToCommD)) * commGain;
            if(debugLog) LOGV("New motor common:", motorComm);

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
            LOG("Navigation is in an unknown state. Halting");
            navState = NAV_STATE_HALT;
            break;
    }
}

void Nav::initVectors() {
    // Discovery time has expired, we should have a valid heading now
    LOG("Discovery time up. Initializing Vectors.");
    double angle = gps->getCourseRadians();
    position = Point(gps->getLat(), gps->getLon()).getVec3d(angle);
    velocity = Vec3d(gps->getSpeedX(), gps->getSpeedY(), mpu->getGZ());
    // int index = setTargetNearest();
    LOG("Waypoints:");
    waypoints.print();
    int index = setTarget(1);
    if(index >= 0) {
        LOGV("Starting waypoint: ", index);
        navState = NAV_STATE_RUN;
    } else {
        LOG("Could not find starting waypoint. Halting");
        navState = NAV_STATE_HALT;
    }
}

void Nav::initErr() {
    updateErr();
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
        float z = (gps_course * GPS_COURSE_WEIGHT) + (position.getZ() * (1.0 - GPS_COURSE_WEIGHT));
        if(gps->getSpeed() > GPS_MIN_SPEED && velocity.getZ() < GPS_MAX_GYRO) {
            // Only use gps heading if we're going fast enough and rotating slow enough
            position = Vec3d(x, y, z);
            LOG("Nav - GPS term with course");
        } else {
            LOG("Nav - GPS term NO course");
            // Vec3d position_gps = Point(gps->getLat(), gps->getLon()).getVec3d(position.getZ());
            position = Vec3d(x, y, position.getZ());
        }
    }
}

void Nav::updateErr() {
    // Update destination errors
    Point target = getTarget();
    if(target) {
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
        LOG("Nav - No target set (target negative)");
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
        LOGV("Setting waypoint ", targetWaypoint);
        return targetWaypoint;
    }
    return -1;
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
        LOG("New waypoint");
        LOGV("Lat: ", wp.getLat());
        LOGV("Lon: ", wp.getLon());
        return size - 1;
    } else {
        LOG("Max waypoints filled");
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
        LOG("No waypoints loaded");
        return -1;
    }
    int closest = -1;
    double closestDist = 100000.0;
    Point thisWP;
    for(int i = 0; i < (int)size; i++) {
        thisWP = waypoints[i];
        double dist = (thisWP - target).getDist();
        LOGV("wp distance: ", dist);
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
        sprintf(printbuff, "WP %d, x:%4.2f, y:%4.2f, lat:%8.6f, lon:%8.6f", i, wp.getX(), wp.getY(), wp.getLat(), wp.getLon());
        LOG(printbuff);
    }
}