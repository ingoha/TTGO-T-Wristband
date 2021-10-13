#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <Wire.h>
#include <pcf8563.h>
#include "translations.hpp"

class Clock {
  private:
    PCF8563_Class* rtc;

    uint8_t current_dayweek = 8;
    uint8_t current_minute = 0;

    const String dayNames[9] = { "ND", "PN", "WT", "SR", "CZ", "PT", "SO", "ND", "\0" };

    /*RTC_DATA_ATTR*/ time_t saved;

  public:
    //void initClock();
    Clock();
    void rtcSleep();
    const int getClockDay();
    const String getClockDayName();
    RTC_Date getClockTime();
    RTC_Date getUTCTime();
    void setTime(RTC_Date datetime);
    const bool isDST(RTC_Date now);

    void saveTime();
    const double diffTime();

  private:
    const time_t getTime();
};

#endif