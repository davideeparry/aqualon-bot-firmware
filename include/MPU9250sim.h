#ifndef MPU9250_H
#define MPU9250_H

#include "Arduino.h"
#include "Wire.h"

class MPU9250 {
    public:
        enum GyroRange
        {
            GYRO_RANGE_250DPS,
            GYRO_RANGE_500DPS,
            GYRO_RANGE_1000DPS,
            GYRO_RANGE_2000DPS
        };
        enum AccelRange
        {
            ACCEL_RANGE_2G,
            ACCEL_RANGE_4G,
            ACCEL_RANGE_8G,
            ACCEL_RANGE_16G
        };

        MPU9250(TwoWire &bus, uint8_t address) : i2c(&bus), address(address) {};

        int begin();

        int readSensor();
        int setAccelRange(AccelRange range);
        int setGyroRange(GyroRange range);

        float getGyroX_rads();
        float getGyroY_rads();
        float getGyroZ_rads();
        float getGyroX_degs();
        float getGyroY_degs();
        float getGyroZ_degs();
        float getAccelX_mss();
        float getAccelY_mss();
        float getAccelZ_mss();

    private:
        char buff[64];
        TwoWire *i2c;
        uint8_t address;
        int16_t gxRaw, gyRaw, gzRaw;
        int16_t axRaw, ayRaw, azRaw;
        float gx, gy, gz;
        float ax, ay, az;
};

#endif