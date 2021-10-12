#ifndef HAL_H
#define HAL_H

#include "battery.hpp"

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
  
  // getter
  public:
    Battery* getBattery();
};

HAL* HAL::instance = nullptr;
#endif