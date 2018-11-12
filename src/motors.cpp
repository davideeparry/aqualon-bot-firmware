#include "Motors.h"


void Motors::init() {
    pinMode(l_direction, OUTPUT);
    pinMode(r_direction, OUTPUT);
}

void Motors::loop() {
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