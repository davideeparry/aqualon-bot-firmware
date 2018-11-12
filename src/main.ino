
#include <Wire.h>
#include "StateService.h"
#include "Communications.h"
#include "Database.h"
//#include "gps/Gps.h"
//#include "mpu/Mpu.h"
//#include "motors/Motors.h"
//#include "power/Power.h"

 // CONTAINS ALL THE SUBSYSTEMS AS GLOBAL CLASSES
void setup() {
   Serial.begin(115200);
   Wire.begin();
   Wire.setClock(400000); 

   Communications::instance().init(Serial);
// this is a service design pattern
   //Mpu::instance().init();
   //Gps::instance().init();
   //Motors::instance().init();
   //Power::instance().init();
   Database::instance().init();
   
}
int counter = 0;
bool flag = false;
void loop() {
    if (Serial.available()) {
       flag = true;
    }
    //Motors::instance().loop();
    if (flag == true) {
        if (counter == 1000000) {
            //Serial.println("hit");
            Communications::instance().isr();
            counter = 0;
            flag = false;
        } else {
            counter++;
        }
    }
   
}
        
        
       
    
