#ifndef GPS
#define GPS 1

#include <TimerOne.h>
#include "TinyGPS++.h"
#include "HardwareSerial.h"
#include "StateService.h"
#include "Communications.h"
#include "Log.h"
#include "Constants.h"
#include <Metro.h>


class Gps 
{
    private:
        HardwareSerial2 gpsSerial;
        TinyGPSPlus gps;
        Metro timer;
        char buf[32];
        float flat, flon;
        unsigned long age;
        Gps() : timer(Metro(100)) {};
    public:
        static Gps& instance() {
            static Gps INSTANCE;
            return INSTANCE;
        }
        // Setup Routine
        void init(int update_interval);
        void isr();
        void update();
        // various get functions
};

#endif
