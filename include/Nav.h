#ifndef NAV_H
#define NAV_H
#include "Gps.h"
#include "Mpu.h"
#include "Motors.h"
#include "Debug.h"

#define NAV_DEFAULT_UPDATE_RATE     100

enum NavigationState
{
    NAV_STATE_STARTUP = 0,
    NAV_STATE_DISCOVERY,
    NAV_STATE_RUN,
    NAV_STATE_HALT
};

class Nav {
    private:
        Metro timer;
        Nav() : timer(Metro(NAV_DEFAULT_UPDATE_RATE)), 
                navState(NAV_STATE_STARTUP),
                numWaypoints(0),
                nextWaypoint(-1) {};

        NavigationState navState;
        Point waypoints[MAX_WAYPOINTS];
        unsigned numWaypoints;
        int nextWaypoint;
        int lastUpdateTime;
        Vec3d position;
        Vec3d velocity;

    public:
        static Nav& instance() {
            static Nav INSTANCE;
            return INSTANCE;
        }
        void init();
        void update();

        void updateVectors(int timeDeltaMS);
        int addWaypoint(double lat, double lon);
        unsigned getNumWaypoints() { return numWaypoints; };
        int getNearestWaypoint();
        int getNearestWaypoint(Point &wp);
        int getNextWaypoint();
        int getNextWaypoint(Point &wp);
        void incrementNextWaypoint();
        void setNavState(NavigationState state) { navState = state; };
        int getNavState() { return navState; };

        Vec3d getPosition() { return position; };
        Vec3d getVelocity() { return velocity; };
};

#endif // NAV_H