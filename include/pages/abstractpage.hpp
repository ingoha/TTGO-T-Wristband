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
    // common constructor
    AbstractPage() {
      hal = HAL::getInstance();
      network = Network::getInstance();
    }
    //
    // draw the page's UI
    //
    virtual void draw(bool initialLoad) = 0;
    //
    // If button is (long) pressed on the page
    //
    virtual void action() = 0;
    //
    // timeout for page in seconds
    //
    virtual const uint16_t timeout() = 0;
};
#endif