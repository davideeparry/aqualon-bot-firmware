#include "Gps.h"
#include "GpsISR.h"
#include "../state/StateService.h"
<<<<<<< HEAD

void Gps::init() {  // GPS TX/RX are plugged into Serial2
   Serial2.begin(9600);
   Timer1.initialize(500000);
=======
#include "../comms/Communications.h"

void Gps::init() {  // GPS TX/RX are plugged into gpsSerial
   gpsSerial.begin(9600);
   Timer1.initialize(50000);
>>>>>>> eventTesting
   Timer1.attachInterrupt(gpsISRWrapper);
};


void Gps::isr() 
{
<<<<<<< HEAD
  //bool newData = false;
  while (Serial2.available()) {
      char c = Serial2.read();
      gps.encode(c);
      if (gps.encode(c)) {// Did a new valid sentence come in?
        gps.f_get_position(&flat, &flon, &age);  
        StateService::instance().gpsLat = flat;
        StateService::instance().gpsLong = flon;
        StateService::instance().gpsAge = age;
        // THERE IS WAY MORE INFO TO GRAB HERE BUT THIS IS JUST AN EXAMPLE
      }
=======
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
>>>>>>> eventTesting
  }
};


