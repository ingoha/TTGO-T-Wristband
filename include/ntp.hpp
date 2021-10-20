#ifndef MYNTP_H
#define MYNTP_H
#include <Arduino.h>
#include <pcf8563.h>

class Ntp {
  public:
    Ntp();
    //
    // get current date&time from ntp server
    // returns a data structure; year is set to zero if error
    RTC_Date syncTime();
};
#endif