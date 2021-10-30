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
    tft->initDrawTemperature();
  }
  else
  {
    tft->refreshDrawTemperature(co2);
  }
}

void PageMqtt::action() {
}

void PageMqtt::onMessage(String topic, String payload)
{
  if(topic == "co2ampel/ppm") {
    Serial.println("ppm: " + payload);
    co2 = payload.toInt();
  }
  
}
