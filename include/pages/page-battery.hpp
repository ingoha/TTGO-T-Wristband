#ifndef PAGE_BATTERY_H
#define PAGE_BATTERY_H
#include "abstractpage.hpp"

class PageBattery : public AbstractPage{
  public:
    // common constructor
    PageBattery(HAL* h, Network* n);

    void draw(bool initialLoad);
    void action();
};
#endif