#include "Motors.h"

void Motors::init() {
    pinMode(PIN_DIR_L, OUTPUT);
    pinMode(PIN_DIR_R, OUTPUT);
    lastUpdateMS = millis();
}

void Motors::setLeft(int l) { 
    if(l > MOTOR_MAX) targetLeft = MOTOR_MAX;
    else if(l < -MOTOR_MAX) targetLeft = -MOTOR_MAX;
    else targetLeft = l;
}

void Motors::setRight(int r) { 
    if(r > MOTOR_MAX) targetRight = MOTOR_MAX;
    else if(r < -MOTOR_MAX) targetRight = -MOTOR_MAX;
    else targetRight = r;
}

void Motors::setDiff(int diff) {
    int common = getCommon();
    setDiffCommon(diff, common);
}

void Motors::setCommon(int common) {
    int diff = getDiff();
    setDiffCommon(diff, common);
}

void Motors::setDiffCommon(int diff, int common) {
    int new_left = common + (diff / 2);
    int new_right = common - (diff / 2);
    if(abs(new_left) > MOTOR_MAX) {
        int over = new_left > 0 ? new_left - MOTOR_MAX : new_left + MOTOR_MAX;
        new_left = new_left - over;
        new_right = new_right - over;
    } else if(abs(new_right) > MOTOR_MAX) {
        int over = new_right > 0 ? new_right - MOTOR_MAX: new_right + MOTOR_MAX;
        new_left = new_left - over;
        new_right = new_right - over;
    }
    targetLeft = new_left;
    targetRight = new_right;
}

int Motors::getLeft() {
    return targetLeft;
}

int Motors::getRight() {
    return targetRight;
}

int Motors::getDiff() {
    int diff = targetLeft - targetRight;
    return diff;
}

int Motors::getCommon() {
    int common = (targetLeft + targetRight) / 2;
    return (targetLeft + targetRight) / 2;
}

// Write motor values to motor control pins
void Motors::update() {
    int now = millis();
    int deltaTime = now - lastUpdateMS;
    lastUpdateMS = now;
    float deltaMotor = slewRate * (float)deltaTime / 1000.0;

    // Slew limiting
    if(targetLeft > (int)actualLeft) {
        actualLeft = min(targetLeft, actualLeft + deltaMotor);
    } else if(targetLeft < (int)actualLeft) {
        actualLeft = max(targetLeft, actualLeft - deltaMotor);
    }
    if(targetRight > (int)actualRight) {
        actualRight = min(targetRight, actualRight + deltaMotor);
    } else if(targetRight < (int)actualRight) {
        actualRight = max(targetRight, actualRight - deltaMotor);
    }

    int mag_left = abs(actualLeft);
    int mag_right = abs(actualRight);
    int dir_left = actualLeft > 0 ? 1 : 0;
    int dir_right = actualRight > 0 ? 1 : 0;
    digitalWrite(PIN_DIR_L, dir_left);
    digitalWrite(PIN_DIR_R, dir_right);
    analogWrite(PIN_SPEED_L, mag_left);
    analogWrite(PIN_SPEED_R, mag_right);
}