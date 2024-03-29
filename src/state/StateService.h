#ifndef STATE
#define STATE 1
#include "Arduino.h"

class StateService {
    StateService() {

    };
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
    double imuYaw;
    double imuPitch;
    double imuRoll;

    // gps
    String gpsStatus;
    int gpsSatellites;
    double gpsHeading;
    double gpsVelocity;
    double gpsLat;
    double gpsLong;
    double gpsAge;
    // 
    String navigationMode;
    double navigationDestinationDecimalDegrees;

    //StateService(); // this constructor will be quite complicated as it needs to know
                    // whether it is recovering from a problem or being freshly deployed.
                    // it will want to try and recover from a copy stored in file storage.
                    
    //bool storeState();  // which brings us to this, which will be used to store the state to fs.

};



#endif