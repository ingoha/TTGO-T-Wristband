#ifndef PAGE_GYRO_H
#define PAGE_GYRO_H
#include <Arduino.h>
#include "pages/abstractpage.hpp"

class PageGyro : public AbstractPage {
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