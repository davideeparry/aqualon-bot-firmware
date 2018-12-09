/*
 * Handle serial input from simulator. 
 * This is replacing the earlier simulator interface which 
 * replicated all of the sensor interfaces in hardware
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Metro.h>
#include <TimeLib.h>
#include "Logging.h"
#include "Motors.h"

#define simSerial Serial4

class Simulator {
    private:
        Simulator() : timer(Metro(20)), debugTimer(Metro(1000)) {};
        Metro timer;
        Metro debugTimer;

    public:
        void init();
        void update();

        static Simulator& instance() {
            static Simulator INSTANCE;
            return INSTANCE;
        }

        double lat;
        double lon;
        float speed;
        float angle;
        float gz;
        float ax;
        float ay;
        unsigned year;
        char month;
        char day;
        char hour;
        char min;
        char sec;
};

time_t simGetTime();

#endif