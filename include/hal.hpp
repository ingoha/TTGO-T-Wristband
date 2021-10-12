#ifndef HAL_H
#define HAL_H

#include "battery.hpp"
class MPU;
#include "mpu.hpp"
#include "clock.hpp"
#include "eeprom.hpp"

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
  
  // getter
  public:
    Battery* getBattery();
    MPU* getMPU();
    Clock* getClock();
    Eeprom* getEEPROM();
};

#endif