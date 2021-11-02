#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <rom/rtc.h>
#include "mpu9250.h"

class MPU {
  private:
    Mpu9250* imu;

  public:
    //void initMPU();
    MPU(TwoWire* wire);
    const int16_t getBearing();
    void calibrate();
    void calibrateGyro();
    int calibrateBearing();
    void mpuSleep();
    const float getTemperature();
    // call this method before accessing values
    void update();

    void sleep();

    typedef struct XYZ
    {
      float x = 0.0;
      float y = 0.0;
      float z = 0.0;
    } xyz;

    // convenience functions
    const float getYaw();
    const float getPitch();
    const float getRoll();
    // get magnetometer data
    // remember to call update() before
    const xyz getMag();
    // get accel data
    // remember to call update() before
    const xyz getAccel();
    // get gyro data
    // remember to call update() before
    const xyz getGyro();

};

#endif