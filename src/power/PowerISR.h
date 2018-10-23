#ifndef POWER_ISR
#define POWER_ISR

#include "../state/globalObjs.h"

void powerISRWrapper() {
    power.isr();
};

#endif