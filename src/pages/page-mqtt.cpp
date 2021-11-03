#include "pages/page-mqtt.hpp"
#include "hal.hpp"

const uint16_t PageMqtt::timeout() { return 60;}

PageMqtt::PageMqtt()
{
  network->getMQTT()->subscribe(this, "co2ampel/ppm");
}

void PageMqtt::draw(bool initialLoading)
{
  TFT_eSPI* tft = hal->getTFT()->getInternalTFT();
  if (initialLoading)
  {
    WIFI* wifi = network->getWIFI();
    wifi->activateWifi();
    wifi->startNetwork();
    tft->fillScreen(TFT_BLACK);
    tft->setTextDatum(BR_DATUM);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->drawString("ppm", tft->width() - 5, tft->height(), 2);
  }
  else
  {
    char co2Text[8] = " ";
    sprintf(co2Text, "%d", co2);
    tft->setTextDatum(TC_DATUM);
    tft->setTextColor(TFT_RED, TFT_BLACK);
    tft->drawString(co2Text, tft->width() / 2, 100, 4);

    //tft->initDrawTemperature();
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
