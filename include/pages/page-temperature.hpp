#ifndef PAGE_TEMPERATURE_H
#define PAGE_TEMPERATURE_H
#include <Arduino.h>
#include "pages/abstractpage.hpp"

class PageTemperature : public AbstractPage {
  private:
    uint32_t timeTemperature = millis();
    int8_t pmenu = -1;

  public:
    void draw(bool initialLoad);
    void action();
    // FIXME add to AbstractPage?
    bool submenu(int8_t press);
};
#endif