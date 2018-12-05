#ifndef GPS
#define GPS 1

#include <Arduino.h>
#include <TimerOne.h>
#include "TinyGPS++.h"
#include "StateService.h"
#include "Communications.h"
#include "Logging.h"
#include "Constants.h"
#include <Metro.h>
#include <TimeLib.h>

#define GPS_DEFAULT_UPDATE_RATE     20   // 50 is bare minimum to keep up with serial buffer
#define GPS_DEBUG_LOG_RATE          1000
#define GPS_TIME_SYNC_RATE          (60 * 60 * 24)  // In seconds, for setSyncInterval
#define GPS_TIMEZONE_OFFSET_PACIFIC (-8)

#if defined(SIMULATION)
#define gpsSerial Serial1
#else
#define gpsSerial Serial4
#endif

class Gps 
{
    private:
        TinyGPSPlus gps;
        Metro timer;
        Metro debugTimer;
        char buf[32];
        float flat, flon;
        unsigned long age;
        Gps() : timer(Metro(GPS_DEFAULT_UPDATE_RATE)),
                debugTimer(Metro(GPS_DEBUG_LOG_RATE)),
                timeZoneOffset(GPS_TIMEZONE_OFFSET_PACIFIC)
                {};

        double courseRadians;
        double speed;
        double lat;
        double lon;
        bool newData;

        // Time
        time_t getTime();
        int timeZoneOffset;

    public:
        static Gps& instance() {
            static Gps INSTANCE;
            return INSTANCE;
        }
        // Setup Routine
        void init();
        void update();

        void setUpdateRate(int update_rate) { timer.interval(update_rate); };

        // various get functions
        double getCourseDegrees() { return 180.0 * courseRadians / PI; };
        double getCourseRadians() { return courseRadians; };
        double getSpeed() { return speed; };
        double getSpeedX() { return speed * cos(courseRadians); };
        double getSpeedY() { return speed * sin(courseRadians); };
        double getLat() { return lat; };
        double getLon() { return lon; };
        bool isNew() { return newData; };
        void markOld() { newData = 0; }
        friend time_t gpsGetTime();  // Wrapper function for TimeLib sync
};

#endif
