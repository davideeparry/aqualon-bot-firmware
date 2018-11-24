#include "Arduino.h"
#include <Wire.h>
#include "StateService.h"
#include "Communications.h"
#include "Database.h"
#include "Gps.h"
#include "Mpu.h"
#include "Motors.h"
#include "Nav.h"
#include "Debug.h"
//#include "Power.h"

// Various control and monitoring tools for simulation mode
void debugUpdate();

// CONTAINS ALL THE SUBSYSTEMS AS GLOBAL CLASSES
void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); 
    delay(5000);
    LOG("In setup()");
    Communications::instance().init(Serial);
    // this is a service design pattern
    Mpu::instance().init();
    Gps::instance().init();
    Motors::instance().init();
    //Power::instance().init();
    //Database::instance().init();
    Nav::instance().init();
}

void loop() {
    Gps::instance().update();
    Mpu::instance().update();
    Nav::instance().update();
    Motors::instance().update();
    #ifdef SIMULATION
    debugUpdate();
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
        case 'w':
        {
            int motorComm = Motors::instance().getCommon();
            Motors::instance().setCommon(motorComm + 10);
            break;
        }

        case 's':
        {
            int motorComm = Motors::instance().getCommon();
            Motors::instance().setCommon(motorComm - 10);
            break;
        }

        case 'd':
        {
            int motorDiff = Motors::instance().getDiff();
            Motors::instance().setDiff(motorDiff + 10);
            break;
        }

        case 'a':
        {
            int motorDiff = Motors::instance().getDiff();
            Motors::instance().setDiff(motorDiff - 10);
            break;
        }

        default:
            break;
    }

    // Log system state to monitor
    static Metro logTimer = Metro(2000);
    if(1 == logTimer.check()) {
        Gps* gps = &Gps::instance();
        Nav* nav = &Nav::instance();

        char logBuff[256];
        Vec3d pos = nav->getPosition();
        Vec3d vel = nav->getVelocity();
        int ml = Motors::instance().getLeft();
        int mr = Motors::instance().getRight();
        Point wp = nav->getTarget();
        sprintf(logBuff, "P:[%6.2f,%6.2f,%6.2f], V:[%6.2f,%6.2f,%6.2f], Lat:%8.6f, Lon:%8.6f, M:(%d, %d), ErA: %4.5f, ErD: %4.2f S:%d, WP: (%4.2f,%4.2f)", 
                pos.getX(), pos.getY(), pos.getZ(),
                vel.getX(), vel.getY(), vel.getZ(), gps->getLat(), gps->getLon(),
                ml, mr, nav->getErrAngle(), nav->getErrDist(), 
                nav->getNavState(), wp.getX(), wp.getY());
        LOG(logBuff);
    }
}