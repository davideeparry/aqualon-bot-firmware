#ifndef MPU
#define MPU 1
#include "Arduino.h"
#include "MPU9250.h"
#include "Communications.h"
#include "TimerOne.h"
#include <Metro.h>

class Mpu {
    private:
        MPU9250* IMU;
        Metro timer;
        Mpu() : timer(Metro(100)) {};
    public:
        static Mpu& instance() {
            static Mpu INSTANCE;
            return INSTANCE;
        }
        void init(int update_interval);
        void update();
        void isr();
};

void mpuISRWrapper();

#endif
