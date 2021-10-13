#pragma once

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include "hal.hpp"
#include "network.hpp"
#include "mqtt.hpp"
#include <ESPAsyncWebServer.h>

void setupWiFi();
void configModeCallback(WiFiManager *myWiFiManager);
void activateWifi();
void deactivateWifi();
void startNetwork();
bool WiFiConnected();
void initWiFi();
