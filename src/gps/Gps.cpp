#include "Gps.h"
#include "GpsISR.h"
#include "../state/StateService.h"
#include "../comms/Communications.h"

void Gps::init() {  // GPS TX/RX are plugged into gpsSerial
   gpsSerial.begin(9600);
   Timer1.initialize(50000);
   Timer1.attachInterrupt(gpsISRWrapper);
};


void Gps::isr() 
{
  while (gpsSerial.available()) {
      char c = gpsSerial.read();
     // if (c != '\n') {
        if (gps.encode(c)) {// Did a new valid sentence come in?
          
         // CommunicationCommunications::instance().sendMessage(c);s::instance().sendMessage("encoded");
       /* gps.f_get_position(&flat, &flon, &age);  
        StateService::instance().gpsLat = flat;
        StateService::instance().gpsLong = flon;
        StateService::instance().gpsAge = age;*/
          // THERE IS WAY MORE INFO TO GRAB HERE BUT THIS IS JUST AN EXAMPLE
        }
     // }
      //Communications::instance().sendMessage("hit SERIAL");     
  }
};


