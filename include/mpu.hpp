#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <rom/rtc.h>
#include <mpu9250.h>
#include <MadgwickAHRS.h>

class MPU {
  private:
    Mpu9250* imu;
    // cf. https://github.com/arduino-libraries/MadgwickAHRS/blob/master/examples/Visualize101/Visualize101.ino
    Madgwick* filter;
    const int sampleRateHz = 25;
    unsigned long microsPerReading, microsPrevious;
    float accelScale, gyroScale;
    void setupFilter();
    bool filterEnabled = false;

  public:
    // stop AHRS filter
    void stopFilter();
    // start AHRS filter
    void startFilter();
    // check if AHRS is running
    const bool filterRunning();
    // call this method in loop() to run AHRS
    void updateFilter();
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