#include "Mpu.h"
#include "Log.h"

void Mpu::init(int update_interval) {
    LOG("Initializing MPU")
    IMU = new MPU9250(Wire,0x68);
    int status = IMU->begin();
    if (status < 0) {
        LOG("** IMU initialization failed **");
        /*
        Communications::instance().sendMessage("IMU initialization unsuccessful\n");
        Communications::instance().sendMessage("Check IMU wiring or try cycling power\n");
        Communications::instance().sendMessage("Status: ");
        Communications::instance().sendMessage(status);
        */
    } else {
        LOGV("IMU initialization status: ", status);
        /*
        Communications::instance().sendMessage("Status: ");
        Communications::instance().sendMessage(status);
        */
    } 
    IMU->setAccelRange(MPU9250::ACCEL_RANGE_2G);
    IMU->setGyroRange(MPU9250::GYRO_RANGE_250DPS);
    timer.interval(update_interval);
}

void Mpu::update() {
    if(1 != timer.check()) return;
    int rc = IMU->readSensor();
    if(rc < 0) {
        LOG("** Could not get MPU reading **");
        return;
    }
    StateService* state = &StateService::instance();
    state->imuGX = IMU->getGyroX_rads();
    state->imuGY = IMU->getGyroY_rads();
    state->imuGZ = IMU->getGyroZ_rads();
    state->imuAX = IMU->getAccelX_mss();
    state->imuAY = IMU->getAccelY_mss();
    state->imuAZ = IMU->getAccelZ_mss();
    state->imuHasData = 1;
    LOGV("New MPU angle: ", state->imuGZ)
    /*
    String tmp = "";
    tmp.concat('g');tmp.concat(gX);tmp.concat('\n');
    tmp.concat('g');tmp.concat(gY);tmp.concat('\n');
    tmp.concat('g');tmp.concat(gZ);tmp.concat('\n');
    tmp.concat('m');tmp.concat(mX);tmp.concat('\n');
    tmp.concat('m');tmp.concat(mY);tmp.concat('\n');
    tmp.concat('m');tmp.concat(mZ);tmp.concat('\n');
    tmp.concat('a');tmp.concat(aX);tmp.concat('\n');
    tmp.concat('a');tmp.concat(aY);tmp.concat('\n');
    tmp.concat('a');tmp.concat(aZ);tmp.concat('\n');
    Communications::instance().sendMessage(tmp);*/
}

void Mpu::isr() {
}

void mpuISRWrapper() {
    Mpu::instance().isr();
};
