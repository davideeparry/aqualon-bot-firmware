#include "comms.h"

void commsInit() {
   Serial4.begin(9600);
   pinMode(INTERRUPT_PIN_COMM, INPUT); 
   attachInterrupt(digitalPinToInterrupt(12), recTransmission, RISING); 
}


void recTransmission() {
   char recBuffer[60];
   Serial.println("Start interrupt:");
   int numQueued = Serial4.available();
   if (numQueued == 0) {
    Serial.println("ERROR: No serial data avaliable from Serial4 on transmission interrupt.");
   } else if (numQueued > 60) {
    Serial.println("ERROR: recBuffer overflow!");
    // WANT TO REFACTOR THE WHOLE SERIAL READ CODE SO THAT THE CODE CAN BE LOOPED HERE UNTIL THE 
    // QUEUE IS EMPTY
   } else {
     int i = 0;
     for (i=0; i < numQueued; i++) {
      recBuffer[i] = Serial4.read();
      if (recBuffer[i] == '\0') {
        break;
      }
     }
     if (recBuffer[i] != '\0') {
      Serial.println("ERROR: incomplete transmission received!");
      Serial4.print("Transmission failed.");
      Serial4.flush();
     } else {
      Serial.print("The received message was: ");
      Serial.println(recBuffer);
      Serial4.print("Transmission received.");
      Serial4.flush();
     }
   }
}