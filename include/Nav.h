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
                nextWaypoint(-1),
                navState(NAV_STATE_STARTUP) {};

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

        int addWaypoint(double lat, double lon);
        int getClosestWaypoint();
        void incrementNextWaypoint();
        void setNavState(NavigationState state) { navState = state; };
};

#endif // NAV_H