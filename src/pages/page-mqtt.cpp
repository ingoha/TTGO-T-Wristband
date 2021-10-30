#include "pages/page-mqtt.hpp"
#include "hal.hpp"

PageMqtt::PageMqtt()
{
  network->getMQTT()->subscribe(this, "co2ampel/ppm");
}

void PageMqtt::draw(bool initialLoading)
{
  TFT* tft = hal->getTFT();
  if (initialLoading)
  {
    //tft->initDrawTemperature();
  }
}

void PageMqtt::action() {
}

void PageMqtt::onMessage(String topic, String payload)
{
  Serial.println("ppm: " + payload);
}
