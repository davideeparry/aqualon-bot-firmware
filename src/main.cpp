#include "Arduino.h"
#include "Communications.h"
#include "Database.h"
#include "Gps.h"
#include "Mpu.h"
#include "Motors.h"
#include "Nav.h"
#include "Logging.h"
#include "Simulator.h"
//#include "Power.h"

// Various control and monitoring tools for simulation mode
void debugUpdate();
void debugLog();

// CONTAINS ALL THE SUBSYSTEMS AS GLOBAL CLASSES
void setup() {
    LogInfo("In setup()");
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); 
    delay(3000);
    LogInfo("Starting initialization");
    #if !defined(SIMULATION)
    Database::instance().init();
    #endif
    // Communications::instance().init(Serial);
    // this is a service design pattern
    Mpu::instance().init();
    Gps::instance().init();
    Motors::instance().init();
    //Power::instance().init();
    Nav::instance().init();
    setLogLevelComms(NONE);
    #if !defined(SIMULATION)
    setLogLevelDatabase(INFO);
    #else
    setLogLevelDatabase(NONE);
    Simulator::instance().init();
    #endif
    setLogLevelSerial(DEBUG);
}

void loop() {
    #if defined(SIMULATION)
    Simulator::instance().update();
    #endif
    Gps::instance().update();
    Mpu::instance().update();
    Nav::instance().update();
    Motors::instance().update();
    #if defined(SIMULATION) || defined(LABBUILD)
    debugUpdate();
    #else
    debugLog();
    #endif
}


void debugUpdate() {
    // Get commands
    char input = '\0';
    if(Serial.available()) {
        input = Serial.read();
    }
    
    switch (input)
    {
        case 'r':
        case 'R':
            // Reset nav state to startup
            Nav::instance().setNavState(NAV_STATE_STARTUP);
            break;

        case 'h':
        case 'H':
            // Set nav to halt
            Nav::instance().setNavState(NAV_STATE_HALT);
            break;

        case 'm':
        case 'M':
            // Set nav to halt
            Nav::instance().setNavState(NAV_STATE_MANUAL);
            break;
        
        // Manual motor control commands
        const static float increment = 0.25;
        case 'w':
        {
            float motorComm = Motors::instance().getCommon();
            Motors::instance().setCommon(motorComm + increment);
            LogDebug("Increasing motor from %f to %f", motorComm, motorComm + increment);
            break;
        }

        case 's':
        {
            float motorComm = Motors::instance().getCommon();
            Motors::instance().setCommon(motorComm - increment);
            break;
        }

        case 'd':
        {
            float motorDiff = Motors::instance().getDiff();
            Motors::instance().setDiff(motorDiff + increment);
            break;
        }

        case 'a':
        {
            float motorDiff = Motors::instance().getDiff();
            Motors::instance().setDiff(motorDiff - increment);
            break;
        }

        default:
            break;
    }
    debugLog();
}

void debugLog() {
    // Log system state to monitor
    static Metro logTimer = Metro(2000);
    if(1 == logTimer.check()) {
        Gps* gps = &Gps::instance();
        Nav* nav = &Nav::instance();
        Vec3d pos = nav->getPosition();
        Vec3d vel = nav->getVelocity();
        int ml = Motors::instance().getLeftScaled();
        int mr = Motors::instance().getRightScaled();
        Point wp = nav->getTarget();
        LogInfo("P:[%6.2f,%6.2f,%6.2f], V:[%6.2f,%6.2f,%6.2f], Lat:%8.6f, Lon:%8.6f, M:(%d, %d), ErA: %4.5f, ErD: %4.2f S:%d, WP: (%4.2f,%4.2f)", 
                pos.getX(), pos.getY(), pos.getZ(),
                vel.getX(), vel.getY(), vel.getZ(), gps->getLat(), gps->getLon(),
                ml, mr, nav->getErrAngle(), nav->getErrDist(), 
                nav->getNavState(), wp.getX(), wp.getY());
    }
}