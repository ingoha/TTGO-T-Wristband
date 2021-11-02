#include "pages/page-battery.hpp"
#include "hal.hpp"

void PageBattery::draw(bool initialLoad)
{
  Battery* bat = hal->getBattery();
  float voltage = bat->getVoltage();
  if (initialLoad or abs(voltage - old_voltage) > 0.1)
  {
    // deactivateWifi();
    hal->getTFT()->drawBattery(
      voltage, bat->calcPercentage(voltage), bat->isCharging());
    old_voltage = voltage;
  }
}

void PageBattery::action() {
  
}
