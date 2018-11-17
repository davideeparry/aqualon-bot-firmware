#include "Motors.h"

void Motors::init(int update_interval) {
    timer.interval(update_interval);
    pinMode(PIN_DIR_L, OUTPUT);
    pinMode(PIN_DIR_R, OUTPUT);
}

void Motors::setLeft(int l) { 
    if(l > MOTOR_MAX) left = MOTOR_MAX;
    else if(l < -MOTOR_MAX) left = -MOTOR_MAX;
    else left = l;
}

void Motors::setRight(int r) { 
    if(r > MOTOR_MAX) right = MOTOR_MAX;
    else if(r < -MOTOR_MAX) right = -MOTOR_MAX;
    else right = r;
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
    int new_left = common + diff;
    int new_right = common - right;
    if(abs(new_left) > MOTOR_MAX) {
        int over = new_left > 0 ? new_left - MOTOR_MAX : new_left + MOTOR_MAX;
        new_left = new_left - over;
        new_right = new_right - over;
    } else if(abs(new_right) > MOTOR_MAX) {
        int over = new_right > 0 ? new_right - MOTOR_MAX: new_right + MOTOR_MAX;
        new_left = new_left - over;
        new_right = new_right - over;
    }
}

int Motors::getLeft() {
    return left;
}

int Motors::getRight() {
    return right;
}

int Motors::getDiff() {
    return left - right;
}

int Motors::getCommon() {
    return (left + right) / 2;
}

// Write motor values to motor control pins
void Motors::update() {
    int mag_left = abs(left);
    int mag_right = abs(right);
    int dir_left = left > 0 ? 1 : 0;
    int dir_right = right > 0 ? 1 : 0;
    digitalWrite(PIN_DIR_L, dir_left);
    digitalWrite(PIN_DIR_R, dir_right);
    analogWrite(PIN_SPEED_L, mag_left);
    analogWrite(PIN_SPEED_R, mag_right);
}