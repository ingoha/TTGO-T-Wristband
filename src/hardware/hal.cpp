#include "hal.hpp"

HAL* HAL::instance = nullptr;

HAL::HAL() {
  battery = new Battery();
  mpu = new MPU();
  clock = new Clock();
  eeprom = new Eeprom();
  tft = new TFT();
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

Clock* HAL::getClock() {
  return clock;
}

Eeprom* HAL::getEEPROM() {
  return eeprom;
}

TFT* HAL::getTFT() {
  return tft;
}