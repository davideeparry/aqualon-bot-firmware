#include "Mpu.h"
#include "MpuISR.h"

void Mpu::init() {
    IMU = new MPU9250(Wire,0x68);
    int status = IMU->begin();
    if (status < 0) {
      /*
      Communications::instance().sendMessage("IMU initialization unsuccessful\n");
      Communications::instance().sendMessage("Check IMU wiring or try cycling power\n");
      Communications::instance().sendMessage("Status: ");
      Communications::instance().sendMessage(status);
      */
    } else {
      /*
      Communications::instance().sendMessage("Status: ");
      Communications::instance().sendMessage(status);
      */
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