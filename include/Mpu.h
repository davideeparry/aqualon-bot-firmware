


#ifndef MPU
#define MPU 1
#include "Arduino.h"
#include "MPU9250.h"
#include "../comms/Communications.h"
#include "TimerOne.h"



class Mpu {
    MPU9250* IMU;
    Mpu() {
        
    };
    public:
    static Mpu& instance() {
        static Mpu INSTANCE;
        return INSTANCE;
    }
    void init();
    void isr();
};


#endif
