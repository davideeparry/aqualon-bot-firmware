/*
 * Functions for retrieving battery voltage and net current readings, 
 * as supplied by the INA219 current shunt power monitor. 
 */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#ifndef _POWER_H
#define _POWER_H value

#define POWER_DEFAULT_UPDATE_RATE_MS    (500)
#define POWER_NUM_READINGS              (256)

/*
 * Struct for storing a power reading, with timestamp
 *
 * Will need to be updated when we have proper time keeping. 
 */
struct power_reading
{
    unsigned long time;
    float V;  // Volts
    float I;  // milliamps
};

/*
 * Initialization. Must be called in setup(). 
 */
void pow_init();

struct power_reading get_reading();

/* 
 * NOT YET IMPLEMENTED
 *
 * Interrupt Service Routine for power monitor. 
 * To be called by a timer based interrupt. 
 */
void pow_isr();

/*
 * Get average of last n voltage readings. 
 */
float pow_get_avg_voltage(unsigned num_readings);

float pow_get_avg_current(unsigned num_readings);

#endif /* ifndef _POWER_H */
