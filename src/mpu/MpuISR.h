#ifndef MPU_ISR
#define MPU_ISR 1

#include "Mpu.h"

void mpuISRWrapper() {
    Mpu::instance().isr();
};

#endif