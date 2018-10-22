
#include <Wire.h>
#include <string>



#include "comms/Communications.h"
#include "power/power.h"
#include "gps/gps.h"


#include "mpu/mpu.h"
#include "motors/motors.h"
#include "state/StateService.h"
#include "state/globalObjs.h"

void setup() {
   Serial.begin(115200);
   Wire.begin();
   Wire.setClock(400000); 
   
   communications.init(stateService); // this is a service design pattern
   gps.init(stateService);
   mpuInit();
}

void loop() {
   motorsLoop();
}
        
        
       
    
