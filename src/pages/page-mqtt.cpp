#include "pages/page-mqtt.hpp"
#include

void PageMQTT::draw(bool initialLoading)
{
  TFT* tft = hal->getTFT();
  if (initialLoading)
  {
    //tft->initDrawTemperature();
  }
}

void PageMqtt::action() {
}
