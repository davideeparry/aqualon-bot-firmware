#ifndef GPS
#define GPS 1

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
