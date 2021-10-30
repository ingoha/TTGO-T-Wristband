#ifndef MYMQTT_H
#define MYMQTT_H
#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <map>

//
// base class for subscribers
//
class AbstractMqttSubscriber {
  public:
    // this method is called if a message for subscriber is received
    virtual void onMessage(String topic, String payload);
};
class MQTT {
  private:
    AsyncMqttClient* mqttClient;
    void onConnect(bool session);
    void onDisconnect(AsyncMqttClientDisconnectReason reason);
    void onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties props, size_t len, size_t index, size_t total);

    // topic, subscriber
    std::map<String,AbstractMqttSubscriber*> subscribers;

  public:
    MQTT();
    void MQTTstatus(const char *payload);
    void MQTTpublish(const char *topic, const char *payload);
    void MQTTinit();
    void MQTTconnect();
    void subscribe(AbstractMqttSubscriber* subscriber, String topic);
};

#endif