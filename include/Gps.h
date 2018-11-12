#ifndef GPS
#define GPS 1

#include <TimerOne.h>
#include "TinyGPS++.h"
#include "HardwareSerial.h"
#include "StateService.h"
#include "Communications.h"


class Gps 
{
    HardwareSerial2 gpsSerial;
    TinyGPSPlus gps;
    char buf[32];
    float flat, flon;
    unsigned long age;
    Gps() {

    };
    public:
    static Gps& instance() {
        static Gps INSTANCE;
        return INSTANCE;
    }
    // Setup Routine
    void init();
    void isr();
    // various get functions
};



#endif
