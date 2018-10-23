/*
 * Functions for retrieving battery voltage and net current readings, 
 * as supplied by the INA219 current shunt power monitor. 
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <TimerThree.h>
#include "../state/StateService.h"

#ifndef _POWER_H
#define _POWER_H value

#define POWER_DEFAULT_UPDATE_RATE_MS    (500)
#define POWER_NUM_READINGS              (256)

struct power_reading
    {
        unsigned long time;
        float V;  // Volts
        float I;  // milliamps
    };


class Power {
    StateService* stateService;
    static Adafruit_INA219 ina219;
    static unsigned update_rate_ms;

    static struct power_reading readings[POWER_NUM_READINGS];
    static unsigned current_index;
    static unsigned power_timer;
    /*
     * Struct for storing a power reading, with timestamp
     *
     * Will need to be updated when we have proper time keeping. 
     */
    struct power_reading get_reading();
    

    public:
    
    void isr();
    /*
     * Initialization. Must be called in setup(). 
     */
    void init(StateService &state);
    
    
    
    
    
    /*
     * Get average of last n voltage readings. 
     */
    float get_avg_voltage(unsigned num_readings);
    
    float get_avg_current(unsigned num_readings);

};


#endif /* ifndef _POWER_H */
