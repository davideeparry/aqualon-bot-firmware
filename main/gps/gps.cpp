#include "gps.h"

void updateGPS() {  // GPS TX/RX are plugged into Serial2
    bool newData = false;
  while (Serial2.available()) {
      char c = Serial2.read();
      gps.encode(c);
      if (gps.encode(c)) {// Did a new valid sentence come in?
        newData = true;
      }
  }
  if (newData) {
     Serial.println("GPS Encoding finished.");
     gps.f_get_position(&flat, &flon, &age);     
  }
}

void gpsInit() {
   Serial2.begin(9600);
   Timer1.initialize(500000);
   Timer1.attachInterrupt(updateGPS);
}

