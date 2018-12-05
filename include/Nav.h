#ifndef NAV_H
#define NAV_H
#include "Gps.h"
#include "Mpu.h"
#include "Motors.h"
#include "Logging.h"
#include <assert.h>

// Navigation
// Some of these should just be defaults for changeable values stored in the nav object
#define MAX_WAYPOINTS       128
#define DISCOVERY_TIME_MS   10000
#define GPS_MIN_SPEED       0.1     // Minimum speed to accept gps heading data
#define GPS_MAX_GYRO        0.05    // Max rotational velocity to accept gps heading data
#define GPS_POS_WEIGHT      0.5
#define GPS_COURSE_WEIGHT   0.4

#define NAV_DEFAULT_UPDATE_RATE     10
#define NAV_DEBUG_RATE              3000

#define NAV_DISTANCE_ARRIVAL        2
#define NAV_DISTANCE_THRESHOLD      5
#define NAV_FORWARD_SPEED           0.6

class WaypointList {
    public:
        WaypointList(): size(0) {};

        int add(double lat, double lon);
        int add(Point wp);
        Point get(unsigned index);
        int getNearest(Point target);
        size_t length();
        void print();

    private:
        Point waypoints[MAX_WAYPOINTS];
        size_t size;
};

enum NavigationState
{
    NAV_STATE_STARTUP = 0,
    NAV_STATE_DISCOVERY,
    NAV_STATE_RUN,
    NAV_STATE_MANUAL,
    NAV_STATE_HALT,
};

class Nav {
    private:
        Nav() : timer(Metro(NAV_DEFAULT_UPDATE_RATE)), 
                debugTimer(Metro(NAV_DEBUG_RATE)),
                navState(NAV_STATE_STARTUP), 
                gps(&Gps::instance()), 
                mpu(&Mpu::instance()),

                // PID loop parameters
                /*
                angleToDiffP(7), 
                angleToDiffD(-40), 
                angleToCommP(-60), 
                angleToCommD(0), 
                distClamp(NAV_DISTANCE_THRESHOLD),       // Max proportional distance error
                angleClamp(0.8),    // For commmon mode only
                distToDiffP(0), 
                distToDiffD(0), 
                distToCommP(NAV_FORWARD_SPEED / NAV_DISTANCE_THRESHOLD),
                distToCommD(0),
                diffGain(1.0),
                commGain(1.0),
                */
                targetWaypoint(-1)
                {};

        Metro timer;
        Metro debugTimer;
        bool debugLog;
        int discoveryStartTime;

        NavigationState navState;

        Gps* gps;
        Mpu* mpu;

        int lastUpdateTime;
        Vec3d position;
        Vec3d velocity;
        Vec3d acceleration;
        float errAngle;
        float lastErrAngle;
        float errDist;
        float lastErrDist;

        Point target;
        int targetWaypoint;

    public:
        static Nav& instance() {
            static Nav INSTANCE;
            return INSTANCE;
        }
        void init();
        void update();

        void initVectors();
        void updateVectors(int timeDeltaMS);
        void initErr();
        void updateErr();

        Point getTarget();
        void setTarget(Point wp);
        int setTarget(unsigned index);
        int setTargetNearest();
        int setTargetNext();

        void setNavState(NavigationState state) { navState = state; };
        int getNavState() { return navState; };

        Vec3d getPosition() { return position; };
        Vec3d getVelocity() { return velocity; };
        Vec3d getAcceleration() { return acceleration; };
        float getErrAngle() { return errAngle; };
        float getErrDist()  { return errDist; };

        WaypointList waypoints;
};

#endif // NAV_H
