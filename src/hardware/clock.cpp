#include "clock.hpp"

Clock::Clock()
{
  rtc = new PCF8563_Class();
  rtc->begin(Wire);
  rtc->check();
}

void Clock::rtcSleep()
{
  rtc->clearTimer();
  rtc->resetAlarm();
  rtc->disableAlarm();
  rtc->disableCLK();
  rtc->disableTimer();
}

const int Clock::getClockDay() {
  RTC_Date now = rtc->getDateTime();
  return rtc->getDayOfWeek(now.day, now.month, now.year);
}

const String Clock::getClockDayName() {
  uint8_t wd = getClockDay();
  if (dayNames[wd] && dayNames[wd][0]) { return dayNames[wd]; }
  return String();
}

RTC_Date Clock::getClockTime()
{
  RTC_Date now = rtc->getDateTime();
  return now;
}

const time_t Clock::getTime() {
  tm saved;
  RTC_Date now = rtc->getDateTime();
  saved.tm_hour = now.hour - (isDST(now) ? 2 : 1);
  saved.tm_min = now.minute;
  saved.tm_sec = now.second;
  saved.tm_mday = now.day;
  saved.tm_mon = now.month - 1;
  saved.tm_year = now.year - 1900;
  saved.tm_isdst = -1;
  return mktime(&saved);
}

void Clock::saveTime() {
  saved = getTime();
}

const double Clock::diffTime() {
  return difftime(getTime(), saved);
}

RTC_Date Clock::getUTCTime()
{
  RTC_Date now = rtc->getDateTime();
  tm timeStructure;
  timeStructure.tm_hour = now.hour;
  timeStructure.tm_min = now.minute;
  timeStructure.tm_sec = now.second;
  timeStructure.tm_mday = now.day;
  timeStructure.tm_mon = now.month - 1;
  timeStructure.tm_year = now.year - 1900;
  timeStructure.tm_isdst = -1;
  if (isDST(now))
  {
    timeStructure.tm_hour = timeStructure.tm_hour - 2;
  }
  else
  {
    timeStructure.tm_hour = timeStructure.tm_hour - 1;
  }
  time_t gmtTime = mktime(&timeStructure);
  tm *gmtStructure = localtime(&gmtTime);
  return RTC_Date(gmtStructure->tm_year + 1900, gmtStructure->tm_mon + 1, gmtStructure->tm_mday, gmtStructure->tm_hour, gmtStructure->tm_min, gmtStructure->tm_sec);
}

void Clock::setTime(RTC_Date datetime)
{
  rtc->setDateTime(datetime);
}

const bool Clock::isDST(RTC_Date now)
{
  uint8_t dayOfWeek = rtc->getDayOfWeek(now.day, now.month, now.year);
  if (now.month < 3 || now.month > 10)
  {
    return false;
  }
  if (now.month > 3 && now.month < 10)
  {
    return true;
  }
  int previousSunday = now.day - dayOfWeek;
  return previousSunday >= 20;
}
