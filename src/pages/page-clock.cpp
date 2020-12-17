
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

void actionCalendar() {
      drawAppointments();
      if (savedStatus[0] != '\0') { status(savedStatus, -1); }
}

void pageCalendar(bool initialLoad) {
  if (initialLoad) { calRefresh = 0; }
  if (millis() - calRefresh > 1000) {
      drawAppointments();
      calRefresh = millis();
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
  } else if (millis() - clockRefresh > 1000) {
    clockRefresh = millis();
    current = getClockTime();
    colon = !colon;
    displayColon(colonX, colon, false);
    if (oldMinute != current.minute) {
      colonX = displayHour(current.hour, current.minute, false);
    }
    if (oldDay != current.day) {
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
    if (appointmentsUpdated) {
      displayAppointments();
      appointmentsUpdated = false;
    }
    oldVoltage = voltage;
    int8_t wifi = WiFi.status() == WL_CONNECTED ? 1 : 0;
    if (wifi != oldWifi) {
      drawStatus('W', wifi > 0, 10);
      oldWifi = wifi;
    }
  }
}

void pageRtc(bool initialLoad) {
  RTC_Date current;
  if (initialLoad) {
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
    if (oldMinuteUTC != current.minute) {
      colonXUTC = displayHour(current.hour, current.minute, true);
    }
    if (oldDayUTC != current.day) {
      displayDate(current.day, current.month, current.year, true);
    }
    oldMinuteUTC = current.minute;
    oldDayUTC = current.day;

    if (grams > 0) {
      grams -= (10 * diffTime()) / 3600;
    }
    if (gramsAdd > 0) {
      float add = (0.5 * diffTime()) / 60;
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
    displayCounter(grams, gramsAdd, grams / (MASS * PER_M));
    saveTime();
  }
}

void actionClock() {
  if (getBusVoltage() < 4.0) {
      msgInfo("NOT CHARGING");
      sleep(1);
      return;
  }
  activateWifi();
  msgInfo("UPDATING TIME...");
  setTime(syncTime());
  // deactivateWifi();
  msgSuccess("TIME UPDATED");
  sleep(3);
}

void actionCounter() { gramsAdd += SHOT_G; }

void zeroCounter() {
  grams = 0;
  gramsAdd = 0;
}

#endif
