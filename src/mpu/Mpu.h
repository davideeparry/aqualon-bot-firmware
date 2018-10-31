


#ifndef MPU
#define MPU 1
<<<<<<< HEAD
=======
#include "Arduino.h"
#include "MPU9250.h"
#include "../comms/Communications.h"
#include "TimerOne.h"
>>>>>>> eventTesting




class Mpu {
<<<<<<< HEAD
    MPU6050 mpu;
    int interruptPin = 11;////se pin 2 on Arduino Uno & most boards

    bool dmpReady = false;  // set true if DMP init was successful
    uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
    uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
    uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
    uint16_t fifoCount;     // count of all bytes currently in FIFO
    uint8_t fifoBuffer[64]; // FIFO storage buffer


    // orientation/motion vars
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorInt16 aa;         // [x, y, z]            accel sensor measurements
    VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
    VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
    VectorFloat gravity;    // [x, y, z]            gravity vector
    float euler[3];         // [psi, theta, phi]    Euler angle container
    float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


    // ================================================================
    // ===               INTERRUPT DETECTION ROUTINE                ===
    // ================================================================

    volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
    
=======
    MPU9250* IMU;
>>>>>>> eventTesting
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
