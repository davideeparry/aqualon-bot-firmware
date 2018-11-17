#include "Nav.h"
#include <Metro.h>

void Nav::init() {

    #ifdef SIMULATION
        // Load simulator coordinates
        addWaypoint(49.245797,-122.958898);
        addWaypoint(49.245799,-122.958306);
        addWaypoint(49.246044,-122.958551);
    #else
    #endif

    navState = NAV_STATE_STARTUP;
}

void Nav::update() {
    // Timer for discovery state
    static int discoveryStartTime;

    if(1 != timer.check()) return;

    Gps* gps = &Gps::instance();
    Mpu* mpu = &Mpu::instance();

    int timeNow = millis();
    int timeDeltaMS = timeNow - lastUpdateTime;
    lastUpdateTime = timeNow;

    if(NAV_STATE_STARTUP == navState) {
        // Wait until we have first GPS reading
        if(gps->isNew()) {
            LOG("Nav: got GPS, switching to discovery");
            Motors::instance().setLeft(30);
            Motors::instance().setRight(30);
            discoveryStartTime = millis();
            gps->markOld();
            navState = NAV_STATE_DISCOVERY;
        }
        return;

    } 
    else if (NAV_STATE_DISCOVERY == navState) {
        if(millis() - discoveryStartTime > DISCOVERY_TIME_MS && 
           gps->isNew() && mpu->hasData()) {
            // Discovery time has expired, we should have a valid heading now
            double angle = gps->getCourseRadians();
            position = Point(gps->getLat(), gps->getLon()).getVec3d(angle);
            double speedX = gps->getSpeed() * cos(angle);
            double speedY = gps->getSpeed() * sin(angle);
            velocity = Vec3d(speedX, speedY, mpu->getGZ());
            nextWaypoint = getClosestWaypoint();
            if(nextWaypoint >= 0) {
                LOGV("Closest waypoint: ", nextWaypoint);
                navState = NAV_STATE_RUN;
            } else {
                LOG("Could not find closest waypoint. Halting");
                navState = NAV_STATE_HALT;
            }
        }
        return;
    } 
    else if (NAV_STATE_RUN == navState) {
        // Estimate new position and velocity
        Vec3d last_position = position;
        Vec3d last_velocity = velocity;
        position = last_position + last_velocity * ((float)timeDeltaMS / 1000.0);
        velocity = Vec3d(last_velocity.getX(), last_velocity.getY(), mpu->getGZ());

        // Add weighted GPS term if new gps is available
        if(gps->isNew()) {
            gps->markOld();
            if(gps->getSpeed() > GPS_MIN_SPEED) {
                // Only use gps heading if we're going fast enough
                Vec3d position_gps = Point(gps->getLat(), gps->getLon()).getVec3d(gps->getCourseRadians());
                position = (position_gps * GPS_WEIGHT) + (position * (1.0 - GPS_WEIGHT));
            } else {
                Vec3d position_gps = Point(gps->getLat(), gps->getLon()).getVec3d(position.getZ());
                position = (position_gps * GPS_WEIGHT) + (position * (1.0 - GPS_WEIGHT));
            }
        }

        // Figure angle and distance to nearest waypoint
        Point wp = waypoints[nextWaypoint];
        Point pos = Point(position);
        Point toTarget = wp - pos;
        double targetDist = toTarget.getDist();
        double targetAngle = toTarget.getAngle();

        // Get closest distance from current heading to target angle
        double errorAngle = targetAngle - position.getZ();
        if(abs(errorAngle) > PI) {
            if(errorAngle < 0) {
                errorAngle = 2 * PI + errorAngle;
            } else {
                errorAngle = errorAngle - 2 * PI;
            }
        }

        // Calculate new motor commands
        // Differential term
        float pTerm = errorAngle * DEFAULT_P_GAIN;
        float dTerm = velocity.getZ();
        float motorDiff = (pTerm + dTerm) * DEFAULT_DRIVE_GAIN;

        // Common term
        float motorCommon = 30;

        Motors::instance().setDiffCommon(motorDiff, motorCommon);

        return;
    } 
    else if (NAV_STATE_HALT == navState) {
        Motors::instance().setLeft(0);
        Motors::instance().setRight(0);
        return;
    } else {
        LOG("Navigation is in an unknown state. Halting");
        navState = NAV_STATE_HALT;
    }
}

int Nav::addWaypoint(double lat, double lon) {
    if(numWaypoints < MAX_WAYPOINTS) {
        waypoints[numWaypoints] = Point(lat, lon);
        numWaypoints++;
        LOG("New waypoint");
        LOGV("Lat: ", lat);
        LOGV("Lon: ", lon);
        return 0;
    } else {
        LOG("Max waypoints filled");
        return 1;
    }
}

// Get closest waypoint to current position
int Nav::getClosestWaypoint() {
    if(numWaypoints == 0) {
        LOG("No waypoints loaded");
        return -1;
    }
    int closest = -1;
    double closestDist = MAXFLOAT;
    for(int i=0; i<numWaypoints; i++) {
        Point position = Point(position);
        Point wp = waypoints[i];
        double dist = (wp - position).getDist();
        LOGV("Waypoint: ", i);
        LOGV("Closest: ", closestDist);
        LOGV("Distance: ", dist);
        if(dist < closestDist) {
            closestDist = dist;
            closest = i;
        }
    }
    return closest;
}

void Nav::incrementNextWaypoint() {
    nextWaypoint = (nextWaypoint + 1) % numWaypoints;
}