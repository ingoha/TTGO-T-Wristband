#include "ntp.hpp"
#include "time.h"

Ntp::Ntp() 
{
  configTime(0, 0, "de.pool.ntp.org");  
// set timezone to Berlin
  setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 3);
  tzset();  
}

RTC_Date Ntp::syncTime()
{
  struct tm timeInfo;
  if (getLocalTime(&timeInfo, 10000)) {
    Serial.println(&timeInfo, "Done: %A, %B %d %Y %H:%M:%S");
  }
  else Serial.println("Error. Unable to download time from NTP server.");
  RTC_Date datetime = RTC_Date(timeInfo.tm_year,
    timeInfo.tm_mon, timeInfo.tm_mday,
    timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  return datetime;
}
