#include "mqtt.hpp"
#include "hal.hpp"
#include "pages/page-clock.hpp"

MQTT::MQTT() {
  mqttClient = new AsyncMqttClient();
  //subscribers = new vector<AbstractMqttSubscriber>();
}

void MQTT::MQTTstatus(const char *payload) {
    mqttClient->publish("esp/watch/band/1/self/status", 0, false, payload);
}

void MQTT::MQTTpublish(const char *topic, const char *payload) {
    String t = "esp/watch/band/1/self/" + String(topic);
    mqttClient->publish(t.c_str(), 0, false, payload);
}

void MQTT::onConnect(bool session) {
  Serial.println("[MQTT] Connected.");
  mqttClient->subscribe("all/#", 2);
  mqttClient->subscribe("esp/#", 2);
  mqttClient->subscribe("esp/watch/#", 2);
  mqttClient->subscribe("esp/watch/band/#", 2);
  mqttClient->subscribe("esp/watch/band/1/#", 2);
  mqttClient->subscribe("watch/#", 2);
  mqttClient->subscribe("appointments/#", 2);
  mqttClient->subscribe("today/#", 2);
  for (std::pair<String, AbstractMqttSubscriber*> element : subscribers) {
    mqttClient->subscribe(element.first.c_str(), 2);
  }
  // status("Connected.", 0);
  Serial.println("[MQTT] Subscribed.");
  RTC_Date now = HAL::getInstance()->getClock()->getClockTime();
  String snow = String(now.hour) + ":" + String(now.minute) + ":" + String(now.second);
  mqttClient->publish("esp/watch/band/1/self/connected", 0, false, snow.c_str());
}

void MQTT::onDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("[MQTT] Disconnected.");
}

void MQTT::onMessage(char *topic, char *opayload,
                   AsyncMqttClientMessageProperties props, size_t len,
                   size_t index, size_t total) {
  char payload[32];
  uint8_t mlen = len < 32 ? len : 32;
  memcpy(payload, opayload, mlen);
  payload[mlen] = '\0';
  // status(payload, -1);
  // if (strlen(payload) != len) { status("Payload error!", -1); return; }
  Serial.printf("[MQTT] %s: %s.\n", topic, payload);
  if (strstr(topic, "self")) { return; }
  if (strstr(topic, "reset")) { esp_restart(); return; }
  if (strstr(topic, "message/store")) {
      saveStatus(payload);
  }
  static uint8_t n = 0;
  
  // notify subscribers
  if(subscribers.find(topic) != subscribers.end()) {
    // FIXME what about multiple subscribers for one topic?
    subscribers[topic]->onMessage(topic, payload);
    Serial.println("[MQTT] Relaying message");
  }
}

void MQTT::MQTTinit() {
  mqttClient->onConnect([this](bool session) {
    this -> onConnect(session);
  });
  mqttClient->onDisconnect([this](AsyncMqttClientDisconnectReason reason) {
    this->onDisconnect(reason);
  });
  mqttClient->onMessage([this](char *topic, char *opayload,
                   AsyncMqttClientMessageProperties props, size_t len,
                   size_t index, size_t total) {
    this->onMessage(topic, opayload, props, len, index, total);
  });
  mqttClient->setServer(IPAddress(BROKER), MQTTPORT);
  Serial.println("[MQTT] Setup done.");
}

void MQTT::MQTTconnect() { mqttClient->connect(); }

void MQTT::subscribe(AbstractMqttSubscriber* subscriber, String topic) {
  mqttClient->subscribe(topic.c_str(), 2);
  // FIXME what about multiple subscribers for one topic?
  subscribers.insert(std::pair<String,AbstractMqttSubscriber*>(topic, subscriber));
  Serial.println("[MQTT] subscriber added - topic: " + topic);
}
