
#include "power.h"

static Adafruit_INA219 ina219;
static unsigned update_rate_ms;

static struct power_reading readings[POWER_NUM_READINGS];
static unsigned current_index = 0;
static unsigned power_timer = 0;

void powInit() 
{
    // Zero the readings
    //memset(&readings, 0, sizeof(readings)*POWER_NUM_READINGS);
    ina219.begin();
    update_rate_ms = POWER_DEFAULT_UPDATE_RATE_MS;
    Timer3.initialize();
    Timer3.attachInterrupt(pow_isr);
}

struct power_reading get_reading()
{
    struct power_reading value;
    if(millis() > power_timer + update_rate_ms) {
        current_index = (current_index + 1) % POWER_NUM_READINGS;
        readings[current_index].time = millis();
        readings[current_index].V = ina219.getBusVoltage_V();
        readings[current_index].I = ina219.getCurrent_mA();
        value = readings[current_index];
        power_timer = millis();
    } else {
        value = readings[current_index];
    }
    return value;
}

void pow_isr()
{
    readings[current_index].time = millis();
    readings[current_index].V = ina219.getBusVoltage_V();
    readings[current_index].I = ina219.getCurrent_mA();
    current_index = (current_index + 1) % POWER_NUM_READINGS;
}

float pow_get_avg_voltage(unsigned num_readings)
{
    unsigned start = current_index;
    unsigned i = start;
    unsigned count = 0;
    float sum = 0;
    while(count < POWER_NUM_READINGS) {
        if(readings[i].time == 0) break;
        sum += readings[i].V;
        count++;
        i = i == 0 ? POWER_NUM_READINGS-1 : i-1;
    }
    return sum / (float)count;
}

float pow_get_avg_current(unsigned num_readings)
{
    unsigned start = current_index;
    unsigned i = start;
    unsigned count = 0;
    float sum = 0;
    while(count < POWER_NUM_READINGS) {
        if(readings[i].time == 0) break;
        sum += readings[i].I;
        count++;
        i = i == 0 ? POWER_NUM_READINGS-1 : i-1;
    }
    return sum / (float)count;
}
