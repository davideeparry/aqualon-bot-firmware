rom #include "libraries/I2Cdev.h"

#include "libraries/MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
// POWER HEADERS
#include <TimerThree.h>
#include "power/power.h"
float v_avg;
float i_avg;
struct power_reading reading;

// COMM HEADERS
#define INTERRUPT_PIN_COMM 
#define commPort Serial4



// GPS HEADERS
#include <TimerOne.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
TinyGPS gps;
#define gpsPort Serial2
char buf[32];
float flat, flon;
unsigned long age;
//

// MOTOR HEADERS
int left = 0;
int right = 0;
int l_speed = 2;
int r_speed = 3;
int r_direction = 4;
int l_direction = 5;
//


//MPU HEADERS
MPU6050 mpu;
#define INTERRUPT_PIN_MPU 11  // use pin 2 on Arduino Uno & most boards

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer


// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================




void updatePower() {
    power_reading reading = get_reading();   // Get the most recent reading, or a new one if it's stale
    v_avg = pow_get_avg_voltage(10);  // Get mean of last 10 readings
    i_avg = pow_get_avg_current(10);

    Serial.print("Time:    "); Serial.println(reading.time);
    Serial.print("Voltage: "); Serial.print(reading.V); Serial.print("  Avg:   "); Serial.println(v_avg);
    Serial.print("Current: "); Serial.print(reading.I); Serial.print("  Avg:   "); Serial.println(i_avg);
    Serial.println(" ");
}

void tranmissionReady() {
   char recBuffer[60];
   Serial.println("Start interrupt:");
   int numQueued = Serial4.available();
   if (numQueued == 0) {
    Serial.println("ERROR: No serial data avaliable from Serial4 on transmission interrupt.");
   } else if (numQueued > 60) {
    Serial.println("ERROR: recBuffer overflow!");
    // WANT TO REFACTOR THE WHOLE SERIAL READ CODE SO THAT THE CODE CAN BE LOOPED HERE UNTIL THE 
    // QUEUE IS EMPTY
   } else {
     int i = 0;
     for (i=0; i < numQueued; i++) {
      recBuffer[i] = Serial4.read();
      if (recBuffer[i] == '\0') {
        break;
      }
     }
     if (recBuffer[i] != '\0') {
      Serial.println("ERROR: incomplete transmission received!");
      Serial4.print("Transmission failed.");
      Serial4.flush();
     } else {
      Serial.print("The received message was: ");
      Serial.println(recBuffer);
      Serial4.print("Transmission received.");
      Serial4.flush();
     }
   }
}
 


void dmpDataReady() {
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;  
   
        // display initial world-frame acceleration, adjusted to remove gravity
        // and rotated based on known orientation from quaternion
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);  
    }  
}
void setup() {
   Serial.begin(115200);
  // POWER SETUP
   pow_init();
   Timer3.initialize();
   Timer3.attachInterrupt(updatePower);
  //

  // COMM SETUP
   Serial4.begin(9600);
   pinMode(INTERRUPT_PIN_COMM, INPUT); 
   attachInterrupt(digitalPinToInterrupt(12), tranmissionReady, RISING); 
   //



  // MOTOR SETUP
  pinMode(l_direction, OUTPUT);
  pinMode(r_direction, OUTPUT);
  //

  // MPU SETUP
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  mpu.initialize();
  pinMode(INTERRUPT_PIN_MPU, INPUT); 
  devStatus = mpu.dmpInitialize();
  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
  if (devStatus == 0) {
      mpu.setDMPEnabled(true);
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_MPU), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();
      dmpReady = true;
      packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
  }
}


void loop() {
  if (left == 0) {
    analogWrite(l_speed, 0);
  } else if (left == -1) {
    analogWrite(l_speed, 60);
    digitalWrite(l_direction, LOW);
  } else if (left == 1) {
    analogWrite(l_speed, 60);
    analogWrite(r_speed, 0);
    digitalWrite(l_direction, HIGH);
  } 

  if (right == 0) {
    analogWrite(r_speed, 0);
  } else if (right == -1) {
    analogWrite(r_speed, 60);
    digitalWrite(r_direction, LOW);
  } else if (right == 1) {
    analogWrite(r_speed, 60);
    digitalWrite(r_direction, HIGH);
  }
}
        
        
       
    
