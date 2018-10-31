
#include "Power.h"
#include "PowerISR.h"
#include "../comms/Communications.h"






void Power::isr() // will need another sensing the charge current
{
    int currentTime = millis();
    int interval;
    if (currentTime >= previousTime) {
        interval = currentTime - previousTime; // this is to deal with rollover
    } else {
        interval = currentTime;
    }
    //ina219.getBusVoltage_V();
    currentAmpH = currentAmpH + interval*ina219.getCurrent_mA();
}

void Power::init() 
{
    // Zero the readings
    //memset(&readings, 0, sizeof(readings)*POWER_NUM_READINGS);
    ina219.begin();
    Timer3.initialize(500000);
    Timer3.attachInterrupt(powerISRWrapper);
}

void Power::initFullSoC()
{
    currentAmpH = 0;
    previousTime = millis();
}

double Power::getSoC() {
    return (maxAmpH-currentAmpH)/maxAmpH;
}

