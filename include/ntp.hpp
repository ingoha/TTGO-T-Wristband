#ifndef MYNTP_H
#define MYNTP_H
#include <Arduino.h>
#include <pcf8563.h>
#include <WiFiUdp.h>
#include <NTP.h>

class Ntp {
  private:
    WiFiUDP* wifiUdp;
    NTP* ntp;
    void initNTP();

  public:
    Ntp();
    RTC_Date syncTime();
};
#endif