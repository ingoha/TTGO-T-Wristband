#include "ntp.hpp"

Ntp::Ntp() 
{
  wifiUdp = new WiFiUDP();
  ntp = new NTP(*wifiUdp);
}

void Ntp::initNTP()
{
  ntp->ruleDST("CEST", Last, Sun, Mar, 2, 120);
  ntp->ruleSTD("CET", Last, Sun, Oct, 3, 60);
  ntp->begin();
  ntp->update();
}

RTC_Date Ntp::syncTime()
{
  initNTP();
  RTC_Date datetime = RTC_Date(ntp->year(), ntp->month(), ntp->day(), 
    ntp->hours(), ntp->minutes(), ntp->seconds());
  ntp->stop();
  return datetime;
}
