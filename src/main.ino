
#include <Wire.h>
#include "state/StateService.h"
#include "comms/Communications.h"
#include "db/Database.h"
#include "gps/Gps.h"
//#include "mpu/Mpu.h"
#include "motors/Motors.h"
#include "power/Power.h"

 // CONTAINS ALL THE SUBSYSTEMS AS GLOBAL CLASSES

void setup() {
   Serial.begin(115200);
   Wire.begin();
   Wire.setClock(400000); 

   Communications::instance().init();
// this is a service design pattern
   //Mpu::instance().init();
   Gps::instance().init();
   Motors::instance().init();
   Power::instance().init();
   Database::instance().init();
}

void loop() {
   Motors::instance().loop();
}
        
        
       
    
