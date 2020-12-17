#include <WiFi.h>
#include <AsyncMqttClient.h>
#include "wristband-tft.hpp"
#include "pages/page-clock.hpp"
#include "calendar.hpp"

void MQTTstatus(const char *payload);
void MQTTpublish(const char *topic, const char *payload);
void MQTTonConnect(bool session);
void MQTTonDisconnect(AsyncMqttClientDisconnectReason reason);
void MQTTonSubscribe(uint16_t id, uint8_t qos);
void MQTTonUnsubscribe(uint16_t id);
void MQTTonMessage(char* topic, char* payload, AsyncMqttClientMessageProperties props, size_t len, size_t index, size_t total);
void MQTTonPublish(uint16_t id);
void MQTTinit();
void MQTTconnect();
