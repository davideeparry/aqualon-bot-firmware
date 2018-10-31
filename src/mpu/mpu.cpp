#include "Mpu.h"
#include "MpuISR.h"

void Mpu::init() {
<<<<<<< HEAD
    mpu.initialize();
    pinMode(interruptPin, INPUT); 
    devStatus = mpu.dmpInitialize();
    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        attachInterrupt(digitalPinToInterrupt(interruptPin), mpuISRWrapper, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
=======
    IMU = new MPU9250(Wire,0x68);
    int status = IMU->begin();
    if (status < 0) {
>>>>>>> eventTesting
    } else {
    } 
    Timer1.attachInterrupt(mpuISRWrapper);

}

void Mpu::isr() {
      IMU->readSensor();
      /*String gX = IMU->getGyroX_rads();
      String gY = IMU->getGyroY_rads();
      String gZ = IMU->getGyroZ_rads();
      String mX = IMU->getMagX_uT();
      String mY = IMU->getMagY_uT();
      String mZ = IMU->getMagZ_uT();
      String aX = IMU->getAccelX_mss();
      String aZ = IMU->getAccelY_mss();
      String aY = IMU->getAccelZ_mss();
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