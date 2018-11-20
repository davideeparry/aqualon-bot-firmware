#ifndef NAV_H
#define NAV_H
#include "Gps.h"
#include "Mpu.h"
#include "Motors.h"
#include "Debug.h"

// Navigation
// Some of these should just be defaults for changeable values stored in the nav object
#define MAX_WAYPOINTS       128
#define DISCOVERY_TIME_MS   3000
#define GPS_MIN_SPEED       0.5
#define GPS_WEIGHT          0.5
#define DEFAULT_P_GAIN      0.8
#define DEFAULT_D_GAIN      0
#define DEFAULT_I_GAIN      0
#define DEFAULT_I_CLAMP     (0.1 * PI)
#define DEFAULT_DRIVE_GAIN  (10.0)
#define NAV_DEFAULT_UPDATE_RATE     100

#define NAV_DISTANCE_ARRIVAL        4
#define NAV_DISTANCE_THRESHOLD

class WaypointList {
    public:
        WaypointList(): destination(-1), 
                        numWaypoints(0) {};
        int addWaypoint(double lat, double lon);
        int addWaypoint(Point wp);
        int getWaypoint(int index, Point &wp);
        int getNearestWaypoint(Point target, Point &wp);
        int getDestination(Point &wp);
        int setDestination(unsigned index);
        int incrementDestination();
        int length();
    private:
        Point waypoints[MAX_WAYPOINTS];
        unsigned numWaypoints;
        int destination;
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
        Metro timer;
        Nav() : timer(Metro(NAV_DEFAULT_UPDATE_RATE)), 
                navState(NAV_STATE_STARTUP),
                numWaypoints(0) {};

        NavigationState navState;
        WaypointList waypoints;
        unsigned numWaypoints;
        Point currentDestination;
        int lastUpdateTime;
        Vec3d position;
        Vec3d velocity;
        double lastError;

    public:
        static Nav& instance() {
            static Nav INSTANCE;
            return INSTANCE;
        }
        void init();
        void update();

        void updateVectors(int timeDeltaMS);
        int addWaypoint(double lat, double lon);
        unsigned getNumWaypoints() { return waypoints.length(); };
        int getDestination(Point &wp) { }
        void setNavState(NavigationState state) { navState = state; };
        int getNavState() { return navState; };
        double getLastError() { return lastError; };

        Vec3d getPosition() { return position; };
        Vec3d getVelocity() { return velocity; };
};

#endif // NAV_H