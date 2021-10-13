
#include "pages/page-clock.hpp"

#ifndef H_PAGE_CLOCK
#define H_PAGE_CLOCK

#define MASS 75
#define PER_M 0.7
#define PER_F 0.6
#define SHOT_G 16

unsigned long clockRefresh = 0;
unsigned long calRefresh = 0;
bool colon = true;
uint16_t colonX = 0;
uint8_t oldMinute = 99;
uint8_t oldDay = 99;
bool colonUTC = true;
uint16_t colonXUTC = 0;
uint8_t oldMinuteUTC = 99;
uint8_t oldDayUTC = 99;
float oldVoltage = 10;
int8_t oldWifi = -1;

RTC_DATA_ATTR float grams = 0;
RTC_DATA_ATTR float gramsAdd = 0;

bool appointmentsUpdated = true; // TODO
RTC_DATA_ATTR char savedStatus[32] = { 0 };

void saveStatus(char* status) {
    if (status[0] == '\0' || strlen(status) == 0 || status == NULL) {
        savedStatus[0] = '\0';
        return;
    }
    strncpy(savedStatus, status, 32);
}

void pageClock(bool initialLoad) {
  RTC_Date current;
  float voltage;
  Battery* bat = HAL::getInstance()->getBattery();
  Clock* clk = HAL::getInstance()->getClock();
  TFT* tft = HAL::getInstance()->getTFT();
  if (initialLoad) {
    // deactivateWifi();
    tft->clearScreen();
    current = clk->getClockTime();
    HAL::getInstance()->getTFT()->displayDate(current.day, current.month, current.year, false);
    colonX = HAL::getInstance()->getTFT()->displayHour(current.hour, current.minute, false);
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
    if (appointmentsUpdated) {
      appointmentsUpdated = false;
    }
    oldVoltage = voltage;
    int8_t wifi = WiFi.status() == WL_CONNECTED ? 1 : 0;
    if (wifi != oldWifi) {
      tft->drawStatus('W', wifi > 0, 10);
      oldWifi = wifi;
    }
  }
}

void pageRtc(bool initialLoad) {
  RTC_Date current;
  Clock* clk = HAL::getInstance()->getClock();
  TFT* tft = HAL::getInstance()->getTFT();
  if (initialLoad) {
    // deactivateWifi();
    tft->clearScreen();
    current = clk->getUTCTime();
    tft->displayDate(current.day, current.month, current.year, true);
    colonXUTC = tft->displayHour(current.hour, current.minute, true);
    oldMinuteUTC = current.minute;
    oldDayUTC = current.day;
    clockRefresh = millis();
  } else if (millis() - clockRefresh > 1000) {
    clockRefresh = millis();
    current = clk->getUTCTime();
    colonUTC = !colonUTC;
    tft->displayColon(colonXUTC, colonUTC, true);
    if (oldMinuteUTC != current.minute) {
      colonXUTC = tft->displayHour(current.hour, current.minute, true);
    }
    if (oldDayUTC != current.day) {
      tft->displayDate(current.day, current.month, current.year, true);
    }
    oldMinuteUTC = current.minute;
    oldDayUTC = current.day;

    if (grams > 0) {
      grams -= (10 * clk->diffTime()) / 3600;
    }
    if (gramsAdd > 0) {
      float add = (0.5 * clk->diffTime()) / 60;
      if (gramsAdd >= add) {
        grams += add;
        gramsAdd -= add;
      } else {
        grams += gramsAdd;
        gramsAdd = 0;
      }
    }
    if (grams < 0) {
      grams = 0;
    }
    if (gramsAdd < 0) {
      gramsAdd = 0;
    }
    tft->displayCounter(grams, gramsAdd, grams / (MASS * PER_M));
    clk->saveTime();
  }
}

void actionClock() {
  TFT* tft = HAL::getInstance()->getTFT();
  if (HAL::getInstance()->getBattery()->getBusVoltage() < 4.0) {
      tft->msgInfo("NOT CHARGING");
      sleep(1);
      return;
  }
  Network::getInstance()->getWIFI()->activateWifi();
  tft->msgInfo("UPDATING TIME...");
  Network::getInstance()->getNTP()->syncTime();
  // deactivateWifi();
  tft->msgSuccess("TIME UPDATED");
  sleep(3);
}

void actionCounter() { gramsAdd += SHOT_G; }

void zeroCounter() {
  grams = 0;
  gramsAdd = 0;
}

#endif
