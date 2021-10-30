#ifndef PAGE_MQTT_H
#define PAGE_MQTT_H
#include <Arduino.h>
#include "pages/abstractpage.hpp"

class PageMqtt : public AbstractPage, AbstractMqttSubscriber {
  private:
    uint32_t timeBearing = millis();
    uint8_t requests = 0;

    // stored values from bus
    uint32_t co2 = 0;
    float temperature = 0.0;
    //...

  public:
    PageMqtt();
    void draw(bool initialLoading);
    void action();
    void onMessage(String topic, String payload);
};
#endif