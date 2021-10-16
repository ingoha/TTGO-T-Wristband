#ifndef PAGE_HOME_H
#define PAGE_HOME_H
#include "abstractpage.hpp"

class PageHome : public AbstractPage {
  private:
    // FIXME copied q&d from page-clock
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

  public:
    void draw(bool initialLoad);
    void action();
};
#endif