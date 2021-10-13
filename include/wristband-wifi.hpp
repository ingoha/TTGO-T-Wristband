#pragma once

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include "hal.hpp"
class TFT;
//#include "network.hpp"
#include "mqtt.hpp"
#include <ESPAsyncWebServer.h>

class WIFI {
  private:
    WiFiManager* wifiManager;
    TFT* tft;
    bool WiFiState = false;
    AsyncWebServer* server;
    

  public:
    WIFI();
    void setupWiFi();
    void WiFiEvent(WiFiEvent_t event, system_event_info_t info);
    void configModeCallback(WiFiManager *myWiFiManager);
    void activateWifi();
    void deactivateWifi();
    void startNetwork();
    const bool WiFiConnected();
    void initWiFi();
};
