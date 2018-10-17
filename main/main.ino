
#include <Wire.h>
// POWER HEADERS
#include <TimerThree.h>
#include "power/power.h"
#include "gps/gps.h"
#include "comms/comms.h"
#include "mpu/mpu.h"



// MOTOR HEADERS
int left = 0;
int right = 0;
int l_speed = 2;
int r_speed = 3;
int r_direction = 4;
int l_direction = 5;
//




void setup() {
   Serial.begin(115200);
   powInit();
   commInit();
   gpsInit();
   mpuInit();



  // MOTOR SETUP
  pinMode(l_direction, OUTPUT);
  pinMode(r_direction, OUTPUT);
  //

  // MPU SETUP
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  
}


void loop() {
  if (left == 0) {
    analogWrite(l_speed, 0);
  } else if (left == -1) {
    analogWrite(l_speed, 60);
    digitalWrite(l_direction, LOW);
  } else if (left == 1) {
    analogWrite(l_speed, 60);
    analogWrite(r_speed, 0);
    digitalWrite(l_direction, HIGH);
  } 

  if (right == 0) {
    analogWrite(r_speed, 0);
  } else if (right == -1) {
    analogWrite(r_speed, 60);
    digitalWrite(r_direction, LOW);
  } else if (right == 1) {
    analogWrite(r_speed, 60);
    digitalWrite(r_direction, HIGH);
  }
}
        
        
       
    
