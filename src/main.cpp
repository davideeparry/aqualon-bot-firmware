#include "Arduino.h"
#include <Wire.h>
#include "StateService.h"
#include "Communications.h"
#include "Database.h"
#include "Gps.h"
#include "Mpu.h"
#include "Motors.h"
#include "Nav.h"
#include "Log.h"
//#include "Power.h"

 // CONTAINS ALL THE SUBSYSTEMS AS GLOBAL CLASSES
void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(400000); 
    LOG("In setup()");
    Communications::instance().init(Serial);
    // this is a service design pattern
    Mpu::instance().init(500);
    Gps::instance().init(211);
    Motors::instance().init(307);
    //Power::instance().init();
    //Database::instance().init();
   
}

void loop() {
    Gps::instance().update();
    Mpu::instance().update();
    navUpdate();
    Motors::instance().update();
    delay(100);
}
        
        
       
    
