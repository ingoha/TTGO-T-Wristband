#include "pages/page-battery.hpp"
#include "hal.hpp"

const uint16_t PageBattery::timeout() { return 30;}

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
