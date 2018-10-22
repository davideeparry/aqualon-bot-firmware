#ifndef MPU_ISR
#define MPU_ISR

#include "../state/globalObjs.h"

void mpuISRWrapper() {
    mpu.isr();
};

#endif