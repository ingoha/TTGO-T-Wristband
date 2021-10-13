#ifndef MYMQTT_H
#define MYMQTT_H
#include <WiFi.h>
#include <AsyncMqttClient.h>

class MQTT {
  private:
    AsyncMqttClient* mqttClient;
    void onConnect(bool session);
    void onDisconnect(AsyncMqttClientDisconnectReason reason);
    void onMessage(char* topic, char* payload, AsyncMqttClientMessageProperties props, size_t len, size_t index, size_t total);

  public:
    MQTT();
    void MQTTstatus(const char *payload);
    void MQTTpublish(const char *topic, const char *payload);
    void MQTTinit();
    void MQTTconnect();
};
#endif