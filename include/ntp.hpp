#ifndef MYNTP_H
#define MYNTP_H
#include <Arduino.h>
#include <pcf8563.h>

class Ntp {
  public:
    Ntp();
    RTC_Date syncTime();
};
#endif