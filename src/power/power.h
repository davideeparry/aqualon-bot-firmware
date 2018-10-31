/*
 * Functions for retrieving battery voltage and net current readings, 
 * as supplied by the INA219 current shunt power monitor. 
 */




#ifndef POWER
#define POWER



#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <TimerThree.h>
#include "../state/StateService.h"
#include "../db/Database.h"

class Power {
    Adafruit_INA219 ina219;
    int maxAmpH = 2200*3600000;
    int currentAmpH = 0;
    int previousTime;
    int writeCounter;
    Power() {

    };
    public:
    static Power& instance() {
        static Power INSTANCE;
        return INSTANCE;
    }
    void isr();
    void init();
    void initFullSoC();
    double getSoC();
};


#endif 
