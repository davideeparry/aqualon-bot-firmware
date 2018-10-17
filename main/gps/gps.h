#include <TimerOne.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
TinyGPS gps;
char buf[32];
float flat, flon;
unsigned long age;

// Setup Routine
void setupGPS();

// Interrupt Routine
void updateGPS();