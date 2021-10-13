#include "pages/page-battery.hpp"
#include "hal.hpp"

void PageBattery::draw(bool initialLoad)
{
  if (initialLoad)
  {
    // deactivateWifi();
    Battery* bat = HAL::getInstance()->getBattery();
    float voltage = bat->getVoltage();
    HAL::getInstance()->getTFT()->drawBattery(
      voltage, bat->calcPercentage(voltage), bat->isCharging());
  }
}

void PageBattery::action() {
  
}
