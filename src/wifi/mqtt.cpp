#include "mqtt.hpp"

AsyncMqttClient mqttClient;

void MQTTstatus(const char *payload) {
    mqttClient.publish("esp/watch/band/1/self/status", 0, false, payload);
}

void MQTTpublish(const char *topic, const char *payload) {
    String t = "esp/watch/band/1/self/" + String(topic);
    mqttClient.publish(t.c_str(), 0, false, payload);
}

void MQTTonConnect(bool session) {
  Serial.println("[MQTT] Connected.");
  mqttClient.subscribe("all/#", 2);
  mqttClient.subscribe("esp/#", 2);
  mqttClient.subscribe("esp/watch/#", 2);
  mqttClient.subscribe("esp/watch/band/#", 2);
  mqttClient.subscribe("esp/watch/band/1/#", 2);
  mqttClient.subscribe("watch/#", 2);
  mqttClient.subscribe("appointments/#", 2);
  mqttClient.subscribe("today/#", 2);
  // status("Connected.", 0);
  Serial.println("[MQTT] Subscribed.");
  RTC_Date now = HAL::getInstance()->getClock()->getClockTime();
  String snow = String(now.hour) + ":" + String(now.minute) + ":" + String(now.second);
  mqttClient.publish("esp/watch/band/1/self/connected", 0, false, snow.c_str());
}

void MQTTonDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("[MQTT] Disconnected.");
}

void MQTTonSubscribe(uint16_t id, uint8_t qos) {}

void MQTTonUnsubscribe(uint16_t id) {}

void MQTTonMessage(char *topic, char *opayload,
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
  TFT* tft = HAL::getInstance()->getTFT();
  if (strstr(topic, "appointment")) {
    if (payload[0] == ' ') {
      n = 0;
    }
    if (payload[0] == 'U') {
      tft->drawAppointments();
    }
    if (payload[0] == 'T' && len > 14) {
        uint8_t hfrom = (payload[1] - '0') * 10 + payload[2] - '0';
        uint8_t mfrom = (payload[4] - '0') * 10 + payload[5] - '0';
        uint8_t hto   = (payload[7] - '0') * 10 + payload[8] - '0';
        uint8_t mto   = (payload[10] - '0') * 10 + payload[11] - '0';
        char* place = strtok((payload+13), "//");
        if (place) {
            saveAppointment(n > 3 ? 3 : n, strtok(NULL, "//"), place, hfrom, mfrom, hto, mto);
        } else {
            saveAppointment(n > 3 ? 3 : n, (payload+13), "-", hfrom, mfrom, hto, mto);
        }
        n++;
    } else if (len > 1) {
        saveAppointment(n > 3 ? 3 : n, payload, "-", 0, 0, 23, 59);
    }
    return;
  }
  tft->status(payload, index);
}

void MQTTonPublish(uint16_t id) {}

void MQTTinit() {
  mqttClient.onConnect(MQTTonConnect);
  mqttClient.onDisconnect(MQTTonDisconnect);
  mqttClient.onSubscribe(MQTTonSubscribe);
  mqttClient.onUnsubscribe(MQTTonUnsubscribe);
  mqttClient.onMessage(MQTTonMessage);
  mqttClient.onPublish(MQTTonPublish);
  mqttClient.setServer(IPAddress(BROKER), MQTTPORT);
  Serial.println("[MQTT] Setup done.");
}

void MQTTconnect() { mqttClient.connect(); }
