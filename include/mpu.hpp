#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <rom/rtc.h>
#include "MPU9250.hpp"
#include "wristband-tft.hpp"
#include "eeprom.hpp"

#include "SparkFunMPU9250-DMP.h"

class MPU {
  private:
    const byte GYRO_CALIBRATION_BASE_ADDRESS = 0x10;
    const byte MAG_CALIBRATION_BASE_ADDRESS = 0x20;
    const byte ACCEL_CALIBRATION_BASE_ADDRESS = 0x30;

    MPU9250lib IMU;
    MPU9250_DMP imud; 

    float aBias[3]; float gBias[3]; float mCal[3];
    uint32_t lastUpdate = 0;

  public:
    //void initMPU();
    MPU();
    const int16_t getBearing();
    int calibrateBearing();
    void calibrateGyro();
    void mpuSleep();
    const float getTemperature();
    const float *getQuaternion();
    void updateMPU();
    void calibrateMPU();

    void gagewatchRead(float *q);
    void updateDMP();
    void getDMP(float *q);
    void imuSleep();
};

#endif