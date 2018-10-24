#include "Gps.h"
#include "GpsISR.h"

void Gps::init(StateService &state) {  // GPS TX/RX are plugged into Serial2
  stateService = &state;
   Serial2.begin(9600);
   Timer1.initialize(500000);
   Timer1.attachInterrupt(gpsISRWrapper);
};


void Gps::isr() 
{
  //bool newData = false;
  while (Serial2.available()) {
      char c = Serial2.read();
      gps.encode(c);
      if (gps.encode(c)) {// Did a new valid sentence come in?
        gps.f_get_position(&flat, &flon, &age);  
        stateService->gpsLat = flat;
        stateService->gpsLong = flon;
        stateService->gpsAge = age;
        // THERE IS WAY MORE INFO TO GRAB HERE BUT THIS IS JUST AN EXAMPLE
      }
  }
};


