#ifndef PAGE_MQTT_H
#define PAGE_MQTT_H
#include <Arduino.h>
#include "pages/abstractpage.hpp"

class PageMqtt : public AbstractPage, AbstractMqttSubscriber {
  private:
    uint32_t timeBearing = millis();
    uint8_t requests = 0;

    // stored values from bus
    int co2;
    float temperature;
    //...

  public:
    PageMqtt();
    void draw(bool initialLoading);
    void action();
    void onMessage(String topic, String payload);
};
#endif