#ifndef STATESERVICE_H
#define STATESERVICE_H
#include "Arduino.h"
#include "Constants.h"
#include "Point.h"
#include "Vec3d.h"
#include "Log.h"

enum NavigationState {
    NAV_STATE_STARTUP = 0,
    NAV_STATE_DISCOVERY,
    NAV_STATE_RUN,
    NAV_STATE_HALT
};

class StateService {
    private:
        StateService() : navState(NAV_STATE_STARTUP), nextWaypoint(-1) {};

    public:
        static StateService& instance() {
            static StateService INSTANCE;
            return INSTANCE;
        }
        // NO REAL THOUGHT IS PUT INTO WHAT THESE ACTUALLY ARE YET, JUST QUICK FROM THE EXISTING ISRs + SOME IMAGINATION
        // There are better ways to do this where we split these catagories up into classes themselves but w/e
        // NOTICE THESE ARE ALL CURRENTLY PUBLIC
        // could have made this class a singleton, going to go with a service for now, so it will get passed
        // as an pointer argument to all other class constructors and only be constructed once.
        // power 
        double powerVInstant; // instant voltage
        double powerIInstant; // instant current
        double powerVAvg; // average voltage
        double powerIAvg; // average current
        double powerTAvg; // time interval used for averages
        double powerOperatingTimeLeft; // time left to operate with normal power consumption

        // position
        double positionHeading; // degrees
        double positionVelocity; 
        double positionDecimalDegrees;

        // motors
        int motorsLeftControl;
        int motorsRightControl;

        // imu
        double imuAX;
        double imuAY;
        double imuAZ;
        double imuGX; 
        double imuGY;
        double imuGZ; // Yaw
        bool imuHasData;

        // gps
        String gpsStatus;
        int gpsSatellites;
        double gpsCourseDegrees;
        double gpsCourseRadians;
        double gpsSpeed;
        double gpsLat;
        double gpsLong;
        double gpsAge;
        bool gpsNew;

        // time
        bool hasTime;
        unsigned long timeOffset;

        // navigation
        NavigationState navState;
        Point waypoints[MAX_WAYPOINTS];
        unsigned numWaypoints;
        int nextWaypoint = -1;
        int lastUpdateTime;
        Vec3d position;
        Vec3d velocity;

        int addWaypoint(double lat, double lon);
        int getClosestWaypoint();
        void incrementNextWaypoint();

        //StateService(); // this constructor will be quite complicated as it needs to know
                        // whether it is recovering from a problem or being freshly deployed.
                        // it will want to try and recover from a copy stored in file storage.
                        
        //bool storeState();  // which brings us to this, which will be used to store the state to fs.
};



#endif