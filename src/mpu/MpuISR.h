#ifndef MPU_ISR
#define MPU_ISR 1

#include "../state/globalObjs.h"

void mpuISRWrapper() {
    mpu.isr();
};

#endif