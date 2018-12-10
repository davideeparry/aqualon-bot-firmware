#ifndef NAV_H
#define NAV_H
#include "Gps.h"
#include "Mpu.h"
#include "Motors.h"
#include "Logging.h"
#include <assert.h>
#include <jled.h>
#include <SD.h>

// Navigation
// Some of these should just be defaults for changeable values stored in the nav object
#define MAX_WAYPOINTS       128
#define WAYPOINT_MIN_DIST   3.0
#define DISCOVERY_TIME_MS   10000
#define GPS_MIN_SPEED       0.20     // Minimum speed to accept gps heading data
#define GPS_MAX_GYRO        0.05    // Max rotational velocity to accept gps heading data
#define GPS_POS_WEIGHT      0.5
#define GPS_COURSE_WEIGHT   0.4

#define NAV_DEFAULT_UPDATE_RATE     10
#define NAV_DEBUG_RATE              1000

#define NAV_DISTANCE_ARRIVAL        2
#define NAV_DISTANCE_THRESHOLD      5
#define NAV_FORWARD_SPEED           0.6

#define NAV_LED_PIN                 23
#define NAV_PERSISTENT_FILE_NAME    "navstate.db"

class WaypointList {
    public:
        WaypointList(): size(0) {};

        int add(double lat, double lon);
        int add(Point wp);
        int remove(size_t index);
        int remove(double lat, double lon);
        int remove(Point wp);
        void clear();
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
    NAV_STATE_STAY
};

class Nav {
    private:
        Nav() : timer(Metro(NAV_DEFAULT_UPDATE_RATE)), 
                debugTimer(Metro(NAV_DEBUG_RATE)),
                navState(NAV_STATE_STARTUP), 
                gps(&Gps::instance()), 
                mpu(&Mpu::instance()),
                targetWaypoint(-1),
                statusLed(JLed(NAV_LED_PIN))
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
        int targetStaypoint;

        JLed statusLed;

    public:
        static Nav& instance() {
            static Nav INSTANCE;
            return INSTANCE;
        }
        void init();
        void update();

        void initVectors();
        void updateVectors(int timeDeltaMS);
        void updateDrive(int timeDeltaMS);
        void initErr();
        void updateErr(Point target);

        Point getTarget();
        void setTarget(Point wp);
        int setTarget(unsigned index);
        int setTargetNearest();
        int setTargetNext();
        void clearWaypoints();
        void makeStayList();

        void setNavState(NavigationState state);
        int getNavState() { return navState; };

        Vec3d getPosition() { return position; };
        Vec3d getVelocity() { return velocity; };
        Vec3d getAcceleration() { return acceleration; };
        float getErrAngle() { return errAngle; };
        float getErrDist()  { return errDist; };

        int saveState();
        int restoreState();

        WaypointList waypoints;
        WaypointList staylist;
};

#endif // NAV_H
