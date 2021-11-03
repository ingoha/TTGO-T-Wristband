#ifndef PAGE_OTA_H
#define PAGE_OTA_H
#include "abstractpage.hpp"

class PageOTA : public AbstractPage {
  private:
    uint32_t otaTimeout = 0;
    bool timeoutDrawn = false;
    void waitOta();

  public:
    void draw(bool initialLoad);
    void action();
    const uint16_t timeout();
};
#endif