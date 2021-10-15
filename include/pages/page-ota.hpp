#ifndef PAGE_OTA_H
#define PAGE_OTA_H
#include "abstractpage.hpp"

class PageOTA : public AbstractPage {
  private:
    uint32_t timeout = 0;
    bool timeoutDrawn = false;
    void waitOta();

  public:
    // common constructor
    PageOTA(HAL* h, Network* n);

    void draw(bool initialLoad);
    void action();
};
#endif