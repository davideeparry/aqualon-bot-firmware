
#include <Wire.h>

#include "power/power.h"
#include "gps/gps.h"
#include "comms/Communications.h"
#include "mpu/mpu.h"
#include "motors/motors.h"


void setup() {
   Serial.begin(115200);
   Wire.begin();
   Wire.setClock(400000); 
   StateService stateService;
   Communications communications;
   communications.init(stateService); // this is a service design pattern
   powInit();
   gpsInit();
   mpuInit();
}

void loop() {
   motorsLoop();
}
        
        
       
    
