#ifndef MPU_H
#define MPU_H
#include "Arduino.h"
#include "Wire.h"
#if defined(SIMULATION)
#include "MPU9250sim.h"
#else
#include "MPU9250.h"
#endif
#include "Communications.h"
#include "Debug.h"
#include <Metro.h>

#define MPU_DEFAULT_UPDATE_RATE     10
#define MPU_DEBUG_LOG_RATE          3000
#define MPU_I2C_ADDRESS             0x68

class Mpu {
    private:
        MPU9250* IMU;
        Metro timer;
        Metro debugTimer;
        Mpu() : timer(Metro(MPU_DEFAULT_UPDATE_RATE)), 
                debugTimer(Metro(MPU_DEBUG_LOG_RATE)), 
                newData(0) {};

        double ax, ay, az, gx, gy, gz;
        bool newData;
        
    public:
        static Mpu& instance() {
            static Mpu INSTANCE;
            return INSTANCE;
        }
        void init();
        void update();

        void setUpdateRate(int update_rate) { timer.interval(update_rate); };

        double getAX() { return ax; };
        double getAY() { return ay; };
        double getAZ() { return az; };
        double getGX() { return gx; };
        double getGY() { return gy; };
        double getGZ() { return gz; };
        bool hasData() { return newData; };
};

#endif
