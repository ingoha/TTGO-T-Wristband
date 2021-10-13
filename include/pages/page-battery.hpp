#ifndef PAGE_BATTERY_H
#define PAGE_BATTERY_H
#include "pages.hpp"
#include "abstractpage.hpp"

class PageBattery : public Pages, public AbstractPage{
  public:
    void draw(bool initialLoad);
    void action();
};
#endif