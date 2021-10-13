#ifndef BATTERY_H
#define BATTERY_H
#include <Arduino.h>

class Battery {
  private:
    const float BATTERY_MIN_V = 3.3;
    const float BATTERY_MAX_V = 4.2;
    int vref;
  
    struct voltagePoint {
      float max;
      uint8_t per;
    };

    const voltagePoint batteryCurve[13] = {
      { 4.20, 100 },
      { 4.12, 97 },
      { 4.10, 95 },
      { 4.05, 90 },
      { 3.95, 80 },
      { 3.82, 60 },
      { 3.75, 40 },
      { 3.68, 20 },
      { 3.60, 10 },
      { 3.48, 5 },
      { 3.30, 2 },
      { 3.00, 0 },
      { 0.00, 0 },
    };

  public: 
    //void setupBattery();
    Battery();
    void setupADC();
    const float getVoltage();
    const float getBusVoltage();
    const uint8_t calcPercentage(float volts);
    void updateBatteryChargeStatus();
    const bool isCharging();
};
#endif