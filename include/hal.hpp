#ifndef HAL_H
#define HAL_H

#include "battery.hpp"
class MPU;
#include "mpu.hpp"

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
  
  // getter
  public:
    Battery* getBattery();
    MPU* getMPU();
};

HAL* HAL::instance = nullptr;
#endif