
#include "pages/page-clock.hpp"

#ifndef H_PAGE_CLOCK
#define H_PAGE_CLOCK

#define MASS 75
#define PER_M 0.7
#define PER_F 0.6
#define SHOT_G 16

unsigned long clockRefresh = 0;
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

void showClock(uint8_t page, uint8_t pages) {
  RTC_Date current;
  float voltage, bvoltage;
  if (millis() - clockRefresh > 1000) {
    current = getClockTime();
    clockRefresh = millis();
    // current = getClockTime();
    tft.setFreeFont(&Orbitron_Light_24);
    /*
    if (!page) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.setTextDatum(TC_DATUM);
      tft.drawString(String(page).c_str(), tft.width() / 2, tft.height() - 50);
    }
    */
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.drawString(String(current.hour < 10 ? "0" : "") + String(current.hour) + ":", 0, 0);
    tft.drawString(String(current.minute < 10 ? "0" : "") + String(current.minute), 0, 25);
    tft.setFreeFont(&orbitron_light7pt7b);
    tft.setTextDatum(TR_DATUM);
    tft.drawString(String(current.day) + ".", tft.width(), 0);
    tft.drawString(String(current.month < 10 ? "0" : "") + String(current.month) + ".", tft.width(), 14);
    tft.drawString(getClockDayName(), tft.width() - 1, 28);
    /* if (lastError[0] != 0) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setTextDatum(TC_DATUM);
      tft.drawString(lastError, tft.width(), 100);
    } */
    tft.setFreeFont(&TomThumb);
    tft.setTextDatum(TR_DATUM);
    tft.drawString(String(current.year), tft.width(), 45);
    tft.setTextDatum(TR_DATUM);
    voltage = getVoltage();
    bvoltage = getBusVoltage();
    tft.drawString(String(bvoltage, 2) + "V " + String(voltage, 2) + "V", tft.width() + 3, 52);
    tft.setTextDatum(TC_DATUM);
    uint8_t percent = calcPercentage(voltage);
    // tft.drawString(String(percent) + "%", tft.width() / 2, 52);
    tft.setTextDatum(TL_DATUM);
    tft.drawString(String(page) + "/" + String(pages), 0, 52);
    tft.setFreeFont(NULL);
    tft.drawLine(0, 58, tft.width(), 58, TFT_GREY);
    uint16_t color = TFT_GREEN;
    if (percent <= 50) { color = TFT_YELLOW; }
    if (percent <= 10) { color = TFT_ORANGE; }
    if (percent <= 5)  { color = TFT_RED; }
    tft.drawLine(0, 58, tft.width() * percent / 100.0, 58, color);
    if (pages > 0) {
      uint8_t pw = tft.width() / (pages + 1);
      uint8_t pr = 0;
      if (pages > 1) { pr = (tft.width() - (pw * pages)) / (pages - 1); }
      for (uint8_t i = 0; i < pages; i++) {
        uint8_t px = i * pw + i * pr;
        tft.drawLine(px, 60, px + pw, 60, i == page ? TFT_CYAN : TFT_WHITE);
      }
    }
  }
}

void pageClock(bool initialLoad) {
  RTC_Date current;
  float voltage;
  if (initialLoad) {
    // deactivateWifi();
    clearScreen();
    current = getClockTime();
    displayDate(current.day, current.month, current.year, false);
    colonX = displayHour(current.hour, current.minute, false);
    oldMinute = current.minute;
    oldDay = current.day;
    clockRefresh = millis();
    voltage = getVoltage();
    displayBatteryValue(voltage, calcPercentage(voltage), isCharging());
    drawBottomBar(calcPercentage(voltage), 0);
    oldVoltage = voltage;
    displayAppointments();
  }
  else if (millis() - clockRefresh > 1000)
  {
    clockRefresh = millis();
    current = getClockTime();
    colon = !colon;
    displayColon(colonX, colon, false);
    if (oldMinute != current.minute)
    {
      colonX = displayHour(current.hour, current.minute, false);
    }
    if (oldDay != current.day)
    {
        displayAppointments();
        displayDate(current.day, current.month, current.year, false);
    }
    oldMinute = current.minute;
    oldDay = current.day;
    voltage = getVoltage();
    if (voltage != oldVoltage) {
        displayBatteryValue(voltage, calcPercentage(voltage), isCharging());
        drawBottomBar(calcPercentage(voltage), 0);
    }
    if (appointmentsUpdated) { displayAppointments(); appointmentsUpdated = false; }
    oldVoltage = voltage;
    int8_t wifi = WiFi.status() == WL_CONNECTED ? 1 : 0;
    if (wifi != oldWifi) {
        drawStatus('W', wifi > 0, 10);
        oldWifi = wifi;
    }
  }
}

void pageRtc(bool initialLoad)
{
  RTC_Date current;
  if (initialLoad)
  {
    // deactivateWifi();
    clearScreen();
    current = getUTCTime();
    displayDate(current.day, current.month, current.year, true);
    colonXUTC = displayHour(current.hour, current.minute, true);
    oldMinuteUTC = current.minute;
    oldDayUTC = current.day;
    clockRefresh = millis();
  } else if (millis() - clockRefresh > 1000) {
    clockRefresh = millis();
    current = getUTCTime();
    colonUTC = !colonUTC;
    displayColon(colonXUTC, colonUTC, true);
    if (oldMinuteUTC != current.minute)
    {
      colonXUTC = displayHour(current.hour, current.minute, true);
    }
    if (oldDayUTC != current.day)
    {
      displayDate(current.day, current.month, current.year, true);
    }
    oldMinuteUTC = current.minute;
    oldDayUTC = current.day;

    if (grams > 0) { grams -= (10 * diffTime()) / 3600; }
    if (gramsAdd > 0) {
      float add = (0.5 * diffTime()) / 60;
      if (gramsAdd >= add) {
        grams += add; gramsAdd -= add;
      } else { grams += gramsAdd; gramsAdd = 0; }
    }
    if (grams < 0) { grams = 0; }
    if (gramsAdd < 0) { gramsAdd = 0; }
    displayCounter(grams, gramsAdd, grams / (MASS * PER_M));
    saveTime();
  }
}

void actionClock()
{
  activateWifi();
  msgInfo("UPDATING TIME...");
  setTime(syncTime());
  // deactivateWifi();
  msgSuccess("TIME UPDATED");
  sleep(3);
}

void actionCounter() {
  gramsAdd += SHOT_G;
}

void zeroCounter() {
  grams = 0;
  gramsAdd = 0;
}

#endif
