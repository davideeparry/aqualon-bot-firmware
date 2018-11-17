#ifndef MOTORS
#define MOTORS 1
#include "StateService.h"
#include "Arduino.h"
#include <Metro.h>

#define MOTOR_MAX   60
#define PIN_SPEED_L 2
#define PIN_SPEED_R 3
#define PIN_DIR_L   4
#define PIN_DIR_R   5

class Motors {
    // Might want to publish to state service so will add it but nothing is being published to it atm
    private:
        int left = 0;
        int right = 0;
        Metro timer = Metro(100);
    
    public:
        Motors() {};
        static Motors& instance() {
            static Motors INSTANCE;
            return INSTANCE;
        }
        void init(int update_time);
        void setLeft(int left);
        void setRight(int right);
        void setDiff(int diff);
        void setCommon(int common);
        void setDiffCommon(int diff, int common);
        int getLeft();
        int getRight();
        int getDiff();
        int getCommon();
        void update();
};


#endif