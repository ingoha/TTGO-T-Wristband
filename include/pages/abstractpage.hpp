#ifndef ABSTRACTPAGE_H
#define ABSTRACTPAGE_H
#include "hal.hpp"
#include "network.hpp"
class AbstractPage {
  protected:
    // Components
    HAL* hal;
    Network* network;
  public:
    //
    // draw the page's UI
    //
    virtual void draw(bool initialLoad);
    //
    // If button is (long) pressed on the page
    //
    virtual void action();
};
#endif