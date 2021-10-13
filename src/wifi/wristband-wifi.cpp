#include "wristband-wifi.hpp"

WIFI::WIFI() {
    tft = HAL::getInstance()->getTFT();
    server = new AsyncWebServer(80);
}

/*
void WIFI::WifiEvent(WiFiEvent_t event, system_event_info_t info, 
      void* this_pointer) {
    WIFI* self = static_cast<WIFI*>(this_pointer);
    self->_WiFiEvent(event, info);
}
*/

void WIFI::WiFiEvent(WiFiEvent_t event, system_event_info_t info) {
    Serial.printf("[WIFI] Event: %d.\n", event);
    switch (event) {
        case SYSTEM_EVENT_STA_CONNECTED:
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            if (!WiFiState) {
                tft->status("Connected.", -1);
                startNetwork();
                WiFiState = true;
            }
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            WiFiState = false;
            tft->status("Disconnected.", -1);
            break;
        default:
            tft->status("WiFi status: ", event);
    }
}

void WIFI::configModeCallback(WiFiManager *myWiFiManager) {
    char apName[50] = " ";
    wifiManager->getConfigPortalSSID().toCharArray(apName, 50);
    tft->wifiManagerAdvice(apName);
    tft->status(apName, -1);
}

/*
void WIFI::configModeCallback(WiFiManager *myWiFiManager, void* this_pointer) {
    WIFI* self = static_cast<WIFI*>(this_pointer);
    self->_configModeCallback(myWiFiManager);
}
*/

const bool WIFI::WiFiConnected() {
    return WiFiState;
}

void WIFI::initWiFi() {
    auto callback = std::bind(&WIFI::WiFiEvent, this, std::placeholders::_1, std::placeholders::_2);
    WiFi.onEvent(callback);
    MQTTinit();
    server->on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });
    server->on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "RST\n");
        esp_restart();
    });
    server->serveStatic("/fs", SPIFFS, "/");
}

void WIFI::setupWiFi() {
    Serial.print("[WIFI] WM mode.\n");
    WiFiManager wifiManager;
    auto callback = std::bind(&WIFI::configModeCallback, this, std::placeholders::_1);
    wifiManager.setAPCallback(callback);
    wifiManager.setBreakAfterConfig(true);
    wifiManager.setConfigPortalTimeout(180);
    wifiManager.autoConnect("T-Wristband");
    sleep(1);
    if (WiFi.status() != WL_CONNECTED) { deactivateWifi(); }
}

void WIFI::activateWifi() {
    // if (WiFiState) { return; }
    if (WiFi.status() == WL_CONNECTED) { return; }
    tft->status("Connecting to WiFi...", -1);
    Serial.print("[WIFI] STA mode.\n");
    WiFi.mode(WIFI_STA);
    uint8_t t = 0;
    for (; t < 100; t++) {
        if (WiFi.status() == WL_CONNECTED) { return; }
        delay(10);
    }
    if (WiFi.status() != WL_CONNECTED) { setupWiFi(); }
}

void WIFI::deactivateWifi() {
    Serial.print("[WIFI] Off.\n");
    WiFi.mode(WIFI_OFF);
}

void WIFI::startNetwork() {
    Serial.println("Connected, network start.\n");
    HAL::getInstance()->getClock()->setTime(
        Network::getInstance()->getNTP()->syncTime());
    MQTTconnect(); 
    server->begin();
}
