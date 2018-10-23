
#include <Wire.h>
#include <string>

#include "comms/Communications.h"
#include "motors/Motors.h"
#include "gps/Gps.h"
#include "power/Power.h"
#include "mpu/Mpu.h"

#include "state/StateService.h"
#include "state/globalObjs.h" // CONTAINS ALL THE SUBSYSTEMS AS GLOBAL CLASSES

void setup() {
   Serial.begin(115200);
   Wire.begin();
   Wire.setClock(400000); 
   
   communications.init(stateService); // this is a service design pattern
   gps.init(stateService);
   motors.init(stateService);
   mpu.init(stateService);
   power.init(stateService);
}

void loop() {
   motors.loop();
}
        
        
       
    
