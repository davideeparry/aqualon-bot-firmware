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

// Function for controlling boat state via serial
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
    StateService* state = &StateService::instance();
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
}