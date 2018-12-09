#include "Mpu.h"

void Mpu::init() {
    LogInfo("Initializing MPU");
    #if !defined(SIMULATION)

    IMU = new MPU9250(Wire, 0x68);
    int status = IMU->begin();
    if (status < 0) {
        LogError("IMU initialization failed, status:", status);
    } else {
        LogInfo("IMU initialization successful, status: ", status);
    } 
    IMU->setAccelRange(MPU9250::ACCEL_RANGE_2G);
    IMU->setGyroRange(MPU9250::GYRO_RANGE_250DPS);

    #endif
}

void Mpu::update() {
    if(1 != timer.check()) return;

    #if defined(SIMULATION)
    static Simulator *sim = &Simulator::instance();
    gz = PI * sim->gz / 180.0;
    ax = sim->ax;
    ay = sim->ay;
    newData = 1;
    #else

    int rc = IMU->readSensor();
    if(rc < 0) {
        LogError("IMU read failed, code: ", rc);
        return;
    }
    gx = IMU->getGyroX_rads();
    gy = IMU->getGyroY_rads();
    gz = IMU->getGyroZ_rads();
    ax = IMU->getAccelX_mss();
    ay = IMU->getAccelY_mss();
    az = IMU->getAccelZ_mss();
    newData = 1;

    #endif

    if(1 == debugTimer.check()) {
        LogInfo("IMU Gyro Z: %f", gz);
    }
}
