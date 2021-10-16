#include "pages/page-battery.hpp"
#include "hal.hpp"

void PageBattery::draw(bool initialLoad)
{
  if (initialLoad)
  {
    // deactivateWifi();
    Battery* bat = hal->getBattery();
    float voltage = bat->getVoltage();
    hal->getTFT()->drawBattery(
      voltage, bat->calcPercentage(voltage), bat->isCharging());
  }
}

void PageBattery::action() {
  
}
