#include "Mpu.h"

void Mpu::init() {
    LOG("Initializing MPU")
    IMU = new MPU9250(Wire,0x68);
    int status = IMU->begin();
    if (status < 0) {
        LOGV("IMU initialization failed, status:", status);
    } else {
        LOGV("IMU initialization successful, status: ", status);
    } 
    IMU->setAccelRange(MPU9250::ACCEL_RANGE_2G);
    IMU->setGyroRange(MPU9250::GYRO_RANGE_250DPS);
}

void Mpu::update() {
    if(1 != timer.check()) return;
    int rc = IMU->readSensor();
    if(rc < 0) {
        LOGV("IMU read failed, code: ", rc);
        return;
    }
    StateService* state = &StateService::instance();
    gx = IMU->getGyroX_rads();
    gy = IMU->getGyroY_rads();
    gz = IMU->getGyroZ_rads();
    ax = IMU->getAccelX_mss();
    ay = IMU->getAccelY_mss();
    az = IMU->getAccelZ_mss();
    newData = 1;
    if(1 == debugTimer.check()) {
        LOGV("IMU Gyro Z: ", gz);
    }
}
