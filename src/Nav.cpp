#include "Nav.h"
#include "Log.h"
#include <Metro.h>

static Metro navTimer = Metro(100);

void navInit(int update_interval) {
    navTimer.interval(update_interval);

    StateService* state = &StateService::instance();

    #ifdef SIMULATION
        // Load simulator coordinates
        state->addWaypoint(49.245797,-122.958898);
        state->addWaypoint(49.245799,-122.958306);
        state->addWaypoint(49.246044,-122.958551);
    #else
    #endif

    state->navState = NAV_STATE_STARTUP;
}

void navUpdate() {
    // Timer for discovery state
    static int discoveryStartTime;

    if(1 != navTimer.check()) return;

    StateService* state = &StateService::instance();
    int timeNow = millis();
    int timeDeltaMS = timeNow - state->lastUpdateTime;
    state->lastUpdateTime = timeNow;

    if(NAV_STATE_STARTUP == state->navState) {
        // Wait until we have first GPS reading
        if(state->gpsNew) {
            Motors::instance().setLeft(30);
            Motors::instance().setRight(30);
            discoveryStartTime = millis();
            state->gpsNew = 0;
            state->navState = NAV_STATE_DISCOVERY;
        }
        return;

    } 
    else if (NAV_STATE_DISCOVERY == state->navState) {
        if(millis() - discoveryStartTime > DISCOVERY_TIME_MS && 
           state->gpsNew && state->imuHasData) {
            // Discovery time has expired, we should have a valid heading now
            double angle = state->gpsCourseRadians;
            state->position = Point(state->gpsLat, state->gpsLong).getVec3d(angle);
            double speedX = state->gpsSpeed * cos(angle);
            double speedY = state->gpsSpeed * sin(angle);
            state->velocity = Vec3d(speedX, speedY, state->imuGZ);
            state->nextWaypoint = state->getClosestWaypoint();
            if(state->nextWaypoint >= 0) {
                state->navState = NAV_STATE_RUN;
            } else {
                LOG("Could not find closest waypoint. Halting");
                state->navState = NAV_STATE_HALT;
            }
        }
        return;
    } 
    else if (NAV_STATE_RUN == state->navState) {
        // Estimate new position and velocity
        Vec3d last_position = state->position;
        Vec3d last_velocity = state->velocity;
        state->position = last_position + last_velocity * ((float)timeDeltaMS / 1000.0);
        state->velocity = Vec3d(last_velocity.getX(), last_velocity.getY(), state->imuGZ);

        // Add weighted GPS term if new gps is available
        if(state->gpsNew) {
            state->gpsNew = 0;
            if(state->gpsSpeed > GPS_MIN_SPEED) {
                // Only use gps heading if we're going fast enough
                Vec3d position_gps = Point(state->gpsLat, state->gpsLong).getVec3d(state->gpsCourseRadians);
                state->position = (position_gps * GPS_WEIGHT) + (state->position * (1.0 - GPS_WEIGHT));
            } else {
                Vec3d position_gps = Point(state->gpsLat, state->gpsLong).getVec3d(state->position.getZ());
                state->position = (position_gps * GPS_WEIGHT) + (state->position * (1.0 - GPS_WEIGHT));
            }
        }

        // Figure angle and distance to nearest waypoint
        Point wp = state->waypoints[state->nextWaypoint];
        Point pos = Point(state->position);
        Point toTarget = wp - pos;
        double targetDist = toTarget.getDist();
        double targetAngle = toTarget.getAngle();

        // Get closest distance from current heading to target angle
        double errorAngle = targetAngle - state->position.getZ();
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
        float dTerm = state->velocity.getZ();
        float motorDiff = (pTerm + dTerm) * DEFAULT_DRIVE_GAIN;

        // Common term
        float motorCommon = 30;

        Motors::instance().setDiffCommon(motorDiff, motorCommon);

        return;
    } 
    else if (NAV_STATE_HALT == state->navState) {
        Motors::instance().setLeft(0);
        Motors::instance().setRight(0);
        return;
    } else {
        LOG("Navigation is in an unknown state. Halting");
        state->navState = NAV_STATE_HALT;
    }
}
