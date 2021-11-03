#ifndef PAGE_BATTERY_H
#define PAGE_BATTERY_H
#include "abstractpage.hpp"

class PageBattery : public AbstractPage {
  private:
    float old_voltage = 0.0;
  public:
    void draw(bool initialLoad);
    void action();
    const uint16_t timeout();
};
#endif