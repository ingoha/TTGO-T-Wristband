#include "hal.hpp"
#include "pins.hpp"

HAL* HAL::instance = nullptr;

HAL::HAL() {
  // FIXME: the init order in the reference code is:
  //    tft, (wire), rtc, mpu, adc
  tft = new TFT();

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);

  clock = new Clock();
  mpu = new MPU(&Wire);
  battery = new Battery();
  eeprom = new Eeprom();
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