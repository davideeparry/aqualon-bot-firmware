#include "Nav.h"
#include <Metro.h>

void Nav::init() {

    #ifdef SIMULATION
        // Load simulator coordinates
        waypoints.addWaypoint(49.245797,-122.958898);
        waypoints.addWaypoint(49.245799,-122.958306);
        waypoints.addWaypoint(49.246044,-122.958551);
    #else
    #endif

    navState = NAV_STATE_STARTUP;
}

void Nav::update() {
    // Timer for discovery state
    static int discoveryStartTime;
    static Metro debugTimer(1000);
    bool debugLog = debugTimer.check();

    if(1 != timer.check()) return;

    Gps* gps = &Gps::instance();
    Mpu* mpu = &Mpu::instance();

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
                Motors::instance().setLeft(30);
                Motors::instance().setRight(30);
                discoveryStartTime = millis();
                gps->markOld();
                navState = NAV_STATE_DISCOVERY;
            }
            break;
        }
        case NAV_STATE_DISCOVERY:
        {
            if(millis() - discoveryStartTime > DISCOVERY_TIME_MS && gps->isNew() && mpu->hasData()) {
                // Discovery time has expired, we should have a valid heading now
                double angle = gps->getCourseRadians();
                position = Point(gps->getLat(), gps->getLon()).getVec3d(angle);
                velocity = Vec3d(gps->getSpeedX(), gps->getSpeedY(), mpu->getGZ());
                Point nextWP;
                int index = waypoints.getNearestWaypoint(Point(position), nextWP);
                if(index >= 0) {
                    LOGV("Closest waypoint: ", index);
                    waypoints.setDestination(index);
                    navState = NAV_STATE_RUN;
                } else {
                    LOG("Could not find closest waypoint. Halting");
                    navState = NAV_STATE_HALT;
                }
            }
            break;
        }
        case NAV_STATE_RUN:
        {
            updateVectors(timeDeltaMS);

            // Figure angle and distance to nearest waypoint
            Point target;
            waypoints.getDestination(target);
            Point pos = Point(position);
            Point toTarget = target - pos;
            double targetDist = toTarget.getDist();
            double targetAngle = toTarget.getAngle();

            // Check if waypoint is reached
            if(targetDist < NAV_DISTANCE_ARRIVAL)
            {
                waypoints.incrementDestination();
                break;
            }

            // Get closest distance from current heading to target angle
            double errorAngle = targetAngle - position.getZ();
            if(abs(errorAngle) > PI) {
                if(errorAngle < 0) {
                    errorAngle = 2 * PI + errorAngle;
                } else {
                    errorAngle = errorAngle - 2 * PI;
                }
            }
            lastError = errorAngle;

            // Calculate new motor commands
            // Differential term
            float pTerm = errorAngle * DEFAULT_P_GAIN;
            float dTerm = velocity.getZ() * DEFAULT_D_GAIN;
            float motorDiff = (pTerm + dTerm) * DEFAULT_DRIVE_GAIN;

            // Common term
            float motorCommon = 30;

            Motors::instance().setDiffCommon(motorDiff, motorCommon);
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

void Nav::updateVectors(int timeDeltaMS) {
    // Estimate new position and velocity
    Gps* gps = &Gps::instance();
    Mpu* mpu = &Mpu::instance();

    Vec3d last_position = position;
    Vec3d last_velocity = velocity;
    position = last_position + last_velocity * ((float)timeDeltaMS / 1000.0);
    velocity = Vec3d(gps->getSpeedX(), gps->getSpeedY(), mpu->getGZ());

    // Add weighted GPS term if new gps is available
    if(gps->isNew()) {
        gps->markOld();
        Vec3d velocity_gps = Vec3d(gps->getSpeedX(), gps->getSpeedY(), mpu->getGZ());
        velocity = (velocity_gps * GPS_WEIGHT) + (velocity_gps * (1.0 - GPS_WEIGHT));
        if(gps->getSpeed() > GPS_MIN_SPEED) {
            // Only use gps heading if we're going fast enough
            Vec3d position_gps = Point(gps->getLat(), gps->getLon()).getVec3d(gps->getCourseRadians());
            position = (position_gps * GPS_WEIGHT) + (position * (1.0 - GPS_WEIGHT));
        } else {
            Vec3d position_gps = Point(gps->getLat(), gps->getLon()).getVec3d(position.getZ());
            position = (position_gps * GPS_WEIGHT) + (position * (1.0 - GPS_WEIGHT));
        }
    }
}


// Waypoints functions

int WaypointList::addWaypoint(double lat, double lon) {
        Point newWP(lat, lon);
        return addWaypoint(newWP);
}

int WaypointList::addWaypoint(Point wp) {
    if(numWaypoints < MAX_WAYPOINTS) {
        waypoints[numWaypoints] = wp;
        numWaypoints++;
        LOG("New waypoint");
        LOGV("Lat: ", wp.getLat());
        LOGV("Lon: ", wp.getLon());
        return 0;
    } else {
        LOG("Max waypoints filled");
        return 1;
    }
}

int WaypointList::getWaypoint(int index, Point &wp) {
    if(index > 0 && numWaypoints > 0 && destination >= 0) {
        wp = waypoints[index];
        return 0;
    }
    return 1;
}

int WaypointList::getNearestWaypoint(Point target, Point &wp) {
    if(numWaypoints == 0) {
        LOG("No waypoints loaded");
        return -1;
    }
    int closest = -1;
    double closestDist = 100000.0;
    for(int i = 0; i < (int)numWaypoints; i++) {
        Point wp = waypoints[i];
        double dist = (wp - target).getDist();
        LOGV("wp distance: ", dist);
        if(dist < closestDist) {
            closestDist = dist;
            closest = i;
        }
    }
    return closest;
}

int WaypointList::getDestination(Point &wp) {
    return getWaypoint(destination, wp);
}

int WaypointList::setDestination(unsigned index) {
    if((int)index < numWaypoints) {
        destination = index;
        return 0;
    }
    return 1;
}

int WaypointList::incrementDestination() {
    if(numWaypoints > 0) {
        destination = (destination + 1) % numWaypoints;
        return 0;
    }
    return -1;
}

int WaypointList::length() {
    return numWaypoints;
}