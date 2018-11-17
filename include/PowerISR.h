#ifndef POWER_ISR
#define POWER_ISR 1

#include "Power.h"

void powerISRWrapper() {
    Power::instance().isr();
};

#endif