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
    int calibrateBearing();
    void calibrateGyro();
    void mpuSleep();
    const float getTemperature();
    const float *getQuaternion();
    void update();
    void calibrate();

    // 4=yaw, 5=pitch, 6=roll
    void getDMP(float *q);
    void sleep();

    // convenience functions
    const float getYaw();
    const float getPitch();
    const float getRoll();
    const float getMagX();
    const float getMagY();
    const float getMagZ();
};

#endif