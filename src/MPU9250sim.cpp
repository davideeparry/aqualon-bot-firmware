#include "MPU9250sim.h"

int MPU9250::begin() { return 1; }

int MPU9250::readSensor() {
    i2c->beginTransmission(address);
    i2c->write(0x3B);  // ACCEL OUT register
    i2c->endTransmission(false);
    uint8_t quantity = 14;
    uint8_t numBytes = i2c->requestFrom(address, quantity);
    for(uint8_t i=0; i < numBytes; i++) {
        buff[i] = i2c->read();
    }
    axRaw = (((int16_t)buff[0]) << 8) | buff[1];  
    ayRaw = (((int16_t)buff[2]) << 8) | buff[3];
    azRaw = (((int16_t)buff[4]) << 8) | buff[5];
    gxRaw = (((int16_t)buff[8]) << 8) | buff[9];
    gyRaw = (((int16_t)buff[10]) << 8) | buff[11];
    gzRaw = (((int16_t)buff[12]) << 8) | buff[13];

    ax = (float)axRaw * 250.0 / 32768.0;
    ay = (float)ayRaw * 250.0 / 32768.0;
    az = (float)azRaw * 250.0 / 32768.0;
    gx = (float)gxRaw * 250.0 / 32768.0;
    gy = (float)gyRaw * 250.0 / 32768.0;
    gz = (float)gzRaw * 250.0 / 32768.0;
    return 1;
}

int MPU9250::setAccelRange(AccelRange range) { return 1; }

int MPU9250::setGyroRange(GyroRange range) { return 1; }

float MPU9250::getGyroX_rads() { return PI * (float)gx / 180.0; }
float MPU9250::getGyroY_rads() { return PI * (float)gy / 180.0; }
float MPU9250::getGyroZ_rads() { return PI * (float)gz / 180.0; }
float MPU9250::getGyroX_degs() { return gx; }
float MPU9250::getGyroY_degs() { return gy; }
float MPU9250::getGyroZ_degs() { return gz; }
float MPU9250::getAccelX_mss() { return ax; }
float MPU9250::getAccelY_mss() { return ay; }
float MPU9250::getAccelZ_mss() { return az; }
