#ifndef GPS
#define GPS

#include <TimerOne.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include "../state/StateService.h"

class Gps 
{
    TinyGPS gps;
    char buf[32];
    float flat, flon;
    unsigned long age;
    StateService* stateService;
    public:
    // Setup Routine
    void init(StateService &stateService);
    void isr();
    // various get functions
};

#endif
