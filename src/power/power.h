/*
 * Functions for retrieving battery voltage and net current readings, 
 * as supplied by the INA219 current shunt power monitor. 
 */




#ifndef _POWER_H
#define _POWER_H 1

#define POWER_DEFAULT_UPDATE_RATE_MS    (500)
#define POWER_NUM_READINGS              (256)

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <TimerThree.h>
#include "../state/StateService.h"

struct power_reading
    {
        unsigned long time;
        float V;  // Volts
        float I;  // milliamps
    };


class Power {
    Adafruit_INA219 ina219;
    unsigned update_rate_ms;

    struct power_reading readings[POWER_NUM_READINGS];
    unsigned current_index;
    unsigned power_timer;
    /*
     * Struct for storing a power reading, with timestamp
     *
     * Will need to be updated when we have proper time keeping. 
     */
    struct power_reading get_reading();
    

    Power() {

    };
    public:
    static Power& instance() {
        static Power INSTANCE;
        return INSTANCE;
    }
    
    void isr();
    /*
     * Initialization. Must be called in setup(). 
     */
    void init();
    
    
    
    
    
    /*
     * Get average of last n voltage readings. 
     */
    float get_avg_voltage(unsigned num_readings);
    
    float get_avg_current(unsigned num_readings);

};


#endif /* ifndef _POWER_H */
