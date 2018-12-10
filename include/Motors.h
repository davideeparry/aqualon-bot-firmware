#ifndef MOTORS
#define MOTORS 1
#include "Arduino.h"
#include "Logging.h"
#include <Metro.h>

// #define MOTOR_MAX   60
#define MOTOR_OUTPUT_BITS   (8)
#define MOTOR_FULL_SCALE    (1 << MOTOR_OUTPUT_BITS)
#define MOTOR_MAX           (MOTOR_FULL_SCALE * 0.3)
#define MOTOR_MIN           (MOTOR_FULL_SCALE * 0.062)
#if defined(SIMULATION)  // Teensy 3.2 pinout is different
#define PIN_SPEED_L 3
#define PIN_DIR_L   4
#define PIN_SPEED_R 5
#define PIN_DIR_R   6
#else
#define PIN_SPEED_L 2
#define PIN_DIR_L   3
#define PIN_SPEED_R 4
#define PIN_DIR_R   5
#endif

#define MOTOR_DEFAULT_SLEW_RATE     1000.0
#define MOTOR_DEFAULT_UPDATE_RATE   100
#define MOTOR_DEBUG_UPDATE_RATE     3000

class Motors {
    // Might want to publish to state service so will add it but nothing is being published to it atm
    private:
        int targetLeft;   // Target motor value
        int targetRight;
        float actualLeft;      // Actual motor value
        float actualRight;

        Metro timer;
        Metro debugTimer;

        int scaleMotorInput(float in);
    
    public:
        Motors() : targetLeft(0), targetRight(0), actualLeft(0), actualRight(0),
                   timer(Metro(MOTOR_DEFAULT_UPDATE_RATE)), 
                   debugTimer(Metro(MOTOR_DEBUG_UPDATE_RATE)),
                   lastUpdateMS(0), slewRate(MOTOR_DEFAULT_SLEW_RATE) {};

        static Motors& instance() {
            static Motors INSTANCE;
            return INSTANCE;
        }

        int lastUpdateMS;
        float slewRate;

        void init();
        void update();

        void setLeft(float left);
        void setRight(float right);
        void setDiff(float diff);
        void setCommon(float common);
        void setDiffCommon(float diff, float common);

        float getLeft();
        float getRight();
        int getLeftScaled();
        int getRightScaled();
        float getDiff();
        float getCommon();
};


#endif