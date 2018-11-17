#include "MPU9250sim.h"

int MPU9250::begin() { return 1; }

int MPU9250::readSensor() {
    i2c->beginTransmission(address);
    i2c->write(0x3B);  // ACCEL OUT register
    i2c->endTransmission(false);
    int numBytes = i2c->requestFrom(address, 14);
    for(int i=0; i < numBytes; i++) {
        buff[i] = i2c->read();
    }
    ax = (((int16_t)buff[0]) << 8) | buff[1];  
    ay = (((int16_t)buff[2]) << 8) | buff[3];
    az = (((int16_t)buff[4]) << 8) | buff[5];
    gx = (((int16_t)buff[8]) << 8) | buff[9];
    gy = (((int16_t)buff[10]) << 8) | buff[11];
    gz = (((int16_t)buff[12]) << 8) | buff[13];
}

int MPU9250::setAccelRange(AccelRange range) { return 1; }

int MPU9250::setGyroRange(GyroRange range) { return 1; }

float MPU9250::getGyroX_rads() { return PI * (float)gx / 180.0; }
float MPU9250::getGyroY_rads() { return PI * (float)gy / 180.0; }
float MPU9250::getGyroZ_rads() { return PI * (float)gz / 180.0; }
float MPU9250::getAccelX_mss() { return ax; }
float MPU9250::getAccelY_mss() { return ay; }
float MPU9250::getAccelZ_mss() { return az; }
