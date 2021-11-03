#ifndef PAGE_BEARING_H
#define PAGE_BEARING_H
#include <Arduino.h>
#include "pages/abstractpage.hpp"

class PageBearing : public AbstractPage {
  private:
    uint32_t timeBearing = millis();
    uint8_t requests = 0;

  public:
    void draw(bool initialLoading);
    void action();
    const uint16_t timeout();
};
#endif