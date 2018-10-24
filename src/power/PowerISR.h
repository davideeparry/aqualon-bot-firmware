#ifndef POWER_ISR
#define POWER_ISR 1

#include "../state/globalObjs.h"

void powerISRWrapper() {
    power.isr();
};

#endif