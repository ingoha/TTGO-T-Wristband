#pragma once

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
class TFT;
#include <ESPAsyncWebServer.h>

class WIFI {
  private:
    WiFiManager* wifiManager;
    TFT* tft;
    bool WiFiState = false;
    AsyncWebServer* server;
    void WiFiEvent(WiFiEvent_t event, system_event_info_t info);
    void configModeCallback(WiFiManager *myWiFiManager);

  public:
    WIFI();
    void setupWiFi();
    void activateWifi();
    void deactivateWifi();
    //
    // Starts network services (NTP, MQTT, Webserver etc.)
    //
    void startNetwork();
    const bool WiFiConnected();
    void initWiFi();
};
