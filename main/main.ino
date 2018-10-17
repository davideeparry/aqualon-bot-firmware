
#include <Wire.h>

#include "power/power.h"
#include "gps/gps.h"
#include "comms/comms.h"
#include "mpu/mpu.h"
#include "motors/motors.h"

void setup() {
   Serial.begin(115200);
   Wire.begin();
   Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
   powInit();
   commInit();
   gpsInit();
   mpuInit();
}

void loop() {
   motorsLoop();
}
        
        
       
    
