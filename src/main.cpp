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

        default:
            break;
    }

    // Log system state to monitor
    static Metro logTimer = Metro(3000);
    if(1 == logTimer.check()) {
        Gps* gps = &Gps::instance();
        Nav* nav = &Nav::instance();

        char logBuff[100];
        Vec3d pos = nav->getPosition();
        Vec3d vel = nav->getVelocity();
        sprintf(logBuff, "Lat: %7.4f (x:%4.3f), Lon: %7.4f (y:%4.2f), \nP:[%6.2f,%6.2f,%6.2f], V:[%6.2f,%6.2f,%6.2f]", 
                gps->getLat(), pos.getX(), gps->getLon(), pos.getY(), 
                pos.getX(), pos.getY(), pos.getZ(),
                vel.getX(), vel.getY(), vel.getZ());
        LOG(logBuff);

        Point wp;
        int wpi = nav->getNextWaypoint(wp);
        int ml = Motors::instance().getLeft();
        int mr = Motors::instance().getRight();
        sprintf(logBuff, "WP %d of %u: (%5.2f,%4.2f), Motors: (%d, %d), State: %d\n", 
                wpi, nav->getNumWaypoints(), wp.getX(), wp.getY(), ml, mr, nav->getNavState());
        LOG(logBuff);
    }
}