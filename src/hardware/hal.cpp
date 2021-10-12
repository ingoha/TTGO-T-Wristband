#include "hal.hpp"
#include "battery.hpp"

HAL::HAL() {
  battery = new Battery();
}

HAL* HAL::getInstance() {
  if(HAL::instance == nullptr) {
    HAL::instance = new HAL();
  }
  return HAL::instance;
}

Battery* HAL::getBattery() {
  return battery;
}

MPU* HAL::getMPU() {
  return mpu;
}