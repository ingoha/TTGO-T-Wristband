#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include "wristband-tft.hpp"
#include "mqtt.hpp"
#include <ESPAsyncWebServer.h>

void setupWiFi();
void configModeCallback(WiFiManager *myWiFiManager);
void activateWifi();
void deactivateWifi();
void startNetwork();
bool WiFiConnected();
void initWiFi();
