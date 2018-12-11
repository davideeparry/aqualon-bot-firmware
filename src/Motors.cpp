#include "Motors.h"

void Motors::init() {
    LogInfo("Motors init");
    pinMode(PIN_DIR_L, OUTPUT);
    pinMode(PIN_DIR_R, OUTPUT);
    analogWriteResolution(MOTOR_OUTPUT_BITS);
    lastUpdateMS = millis();
}

int Motors::scaleMotorInput(float in) {
    int sign = in < 0 ? -1 : 1;
    float ain = min(1.0, abs(in)) * (float)MOTOR_MAX;
    // if(ain < MOTOR_MIN) {
    //     ain = ain < (MOTOR_MIN / 4.0) ? 0.0 : (float)MOTOR_MIN;
    // }
    return (int)ain * sign;
}

void Motors::setLeft(float l) { 
    targetLeft = scaleMotorInput(l);
}

void Motors::setRight(float r) {
    targetRight = scaleMotorInput(r);
}

void Motors::setDiff(float diff) {
    float common = getCommon();
    setDiffCommon(diff, common);
}

void Motors::setCommon(float common) {
    float diff = getDiff();
    setDiffCommon(diff, common);
}

void Motors::setDiffCommon(float diff, float common) {
    // Set calculate motor output for common and differential terms. 
    // Overage protection here aims to preserve the differential term

    static const float mmax = 1.0;

    float new_left = common + (diff / 2);
    float new_right = common - (diff / 2);
    if(abs(new_left) > mmax) {
        float over = new_left > 0 ? new_left - mmax : new_left + mmax;
        new_left = new_left - over;
        new_right = new_right - over;
    } else if(abs(new_right) > mmax) {
        float over = new_right > 0 ? new_right - mmax: new_right + mmax;
        new_left = new_left - over;
        new_right = new_right - over;
    }
    setLeft(new_left);
    setRight(new_right);
}

void Motors::setOverride(int left, int right) {
    targetLeft = left;
    targetRight = right;
}

float Motors::getLeft() {
    return (float)actualLeft / (float)MOTOR_MAX;
}

int Motors::getLeftScaled() {
    return actualLeft;
}

int Motors::getRightScaled() {
    return actualRight;
}

float Motors::getRight() {
    return (float)actualRight / (float)MOTOR_MAX;
}

float Motors::getDiff() {
    int diff = targetLeft - targetRight;
    return (float)diff / (float)MOTOR_MAX;
}

float Motors::getCommon() {
    int common = (targetLeft + targetRight) / 2;
    return (float)common / (float)MOTOR_MAX;
}

// Write motor values to motor control pins
void Motors::update() {
    if(1 != timer.check()) return;

    // Slew limiting
    /*
    int now = millis();
    int deltaTime = now - lastUpdateMS;
    lastUpdateMS = now;
    float deltaMotor = slewRate * (float)deltaTime / 1000.0;
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
    */
    actualLeft = targetLeft;
    actualRight = targetRight;

    int mag_left = min(abs(actualLeft), MOTOR_MAX);
    int mag_right = min(abs(actualRight), MOTOR_MAX);
    int dir_left = actualLeft > 0 ? 1 : 0;
    int dir_right = actualRight > 0 ? 0 : 1;  // Right propellor has opposite twist
    digitalWrite(PIN_DIR_L, dir_left);
    digitalWrite(PIN_DIR_R, dir_right);
    analogWrite(PIN_SPEED_L, mag_left);
    analogWrite(PIN_SPEED_R, mag_right);
}