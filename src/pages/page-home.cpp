#include "pages/page-home.hpp"
#include "hal.hpp"

// was page-clock
void PageHome::draw(bool initialLoad)
{  
  RTC_Date current;
  float voltage;
  Battery* bat = hal->getBattery();
  Clock* clk = hal->getClock();
  TFT* tft = hal->getTFT();
  if (initialLoad) {
    // deactivateWifi();
    tft->clearScreen();
    current = clk->getClockTime();
    tft->displayDate(current.day, current.month, current.year, false);
    colonX = tft->displayHour(current.hour, current.minute, false);
    oldMinute = current.minute;
    oldDay = current.day;
    clockRefresh = millis();
    voltage = bat->getVoltage();
    tft->displayBatteryValue(voltage, bat->calcPercentage(voltage), bat->isCharging());
    tft->drawBottomBar(bat->calcPercentage(voltage), 0);
    oldVoltage = voltage;
  } 
  else if (millis() - clockRefresh > 1000) {
    clockRefresh = millis();
    current = clk->getClockTime();
    colon = !colon;
    tft->displayColon(colonX, colon, false);
    if (oldMinute != current.minute) {
      colonX = tft->displayHour(current.hour, current.minute, false);
    }
    if (oldDay != current.day) {
      tft->displayDate(current.day, current.month, current.year, false);
    }
    oldMinute = current.minute;
    oldDay = current.day;
    voltage = bat->getVoltage();
    if (voltage != oldVoltage) {
      tft->displayBatteryValue(voltage, bat->calcPercentage(voltage), bat->isCharging());
      tft->drawBottomBar(bat->calcPercentage(voltage), 0);
    }
    oldVoltage = voltage;
    int8_t wifi = WiFi.status() == WL_CONNECTED ? 1 : 0;
    if (wifi != oldWifi) {
      tft->drawStatus('W', wifi > 0, 10);
      oldWifi = wifi;
    }
  }
}

// copied from actionClock
void PageHome::action() {
  if (hal->getBattery()->getBusVoltage() < 4.0) {
      hal->getTFT()->msgInfo("NOT CHARGING");
      sleep(1);
      return;
  }
  network->getWIFI()->activateWifi();
  hal->getTFT()->msgInfo("UPDATING TIME...");
  network->getNTP()->syncTime();
  // deactivateWifi();
  hal->getTFT()->msgSuccess("TIME UPDATED");
  sleep(3);
}
