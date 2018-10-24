#ifndef MOTORS
#define MOTORS 1
#include "../state/StateService.h"
#include "Arduino.h"

class Motors {
    // Might want to publish to state service so will add it but nothing is being published to it atm
    StateService* stateService;
    int left = 0;
    int right = 0;
    int l_speed = 2;
    int r_speed = 3;
    int r_direction = 4;
    int l_direction = 5;
    public:
    void init(StateService &stateService);

    void loop();
};

#endif