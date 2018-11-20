#ifndef MOTORS
#define MOTORS 1
#include "StateService.h"
#include "Arduino.h"
#include <Metro.h>

#define MOTOR_MAX   60
#define PIN_SPEED_L 3
#define PIN_SPEED_R 4
#define PIN_DIR_L   5
#define PIN_DIR_R   6

#define MOTOR_DEFAULT_SLEW_RATE     10.0
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

        void setLeft(int left);
        void setRight(int right);
        void setDiff(int diff);
        void setCommon(int common);
        void setDiffCommon(int diff, int common);

        int getLeft();
        int getRight();
        int getDiff();
        int getCommon();
};


#endif