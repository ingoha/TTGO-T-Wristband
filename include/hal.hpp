#ifndef HAL_H
#define HAL_H

#include <Wire.h>

#include "battery.hpp"
#include "mpu.hpp"
#include "clock.hpp"
#include "eeprom.hpp"
#include "wristband-tft.hpp"

class HAL {
  // singleton
  private:
    static HAL* instance;
    HAL();

  public:
    static HAL* getInstance();

  // components
  private:
    Battery* battery;
    MPU* mpu;
    Clock* clock;
    Eeprom* eeprom;
    TFT* tft;
  
  // getter
  public:
    Battery* getBattery();
    MPU* getMPU();
    Clock* getClock();
    Eeprom* getEEPROM();
    TFT* getTFT();
};

#endif