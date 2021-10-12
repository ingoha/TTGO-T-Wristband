#include "pages/page-battery.hpp"

void pageBattery(bool initialLoad)
{
  if (initialLoad)
  {
    // deactivateWifi();
    Battery* bat = HAL::getInstance()->getBattery();
    float voltage = bat->getVoltage();
    drawBattery(voltage, bat->calcPercentage(voltage), bat->isCharging());
  }
}
