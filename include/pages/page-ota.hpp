#ifndef PAGE_OTA_H
#define PAGE_OTA_H
#include "pages.hpp"
#include "abstractpage.hpp"


class PageOTA : public Pages, public AbstractPage {
  private:
    uint32_t timeout = 0;
    bool timeoutDrawn = false;
    void waitOta();

  public:
    void draw(bool initialLoad);
    void action();
};
#endif