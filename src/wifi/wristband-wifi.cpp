#include "wristband-wifi.hpp"
#include "WiFiManager.h"

WiFiManager wifiManager;
bool WiFiState = false;

void configModeCallback(WiFiManager *myWiFiManager) {
    char apName[50] = " ";
    wifiManager.getConfigPortalSSID().toCharArray(apName, 50);
    wifiManagerAdvice(apName);
}

void setupWiFi() {
    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setBreakAfterConfig(true);
    wifiManager.setConfigPortalTimeout(180);
    wifiManager.autoConnect("T-Wristband");
    sleep(1);
    WiFiState = true;
    if (WiFi.status() != WL_CONNECTED) { deactivateWifi(); }
}

void activateWifi() {
    // if (WiFiState) { return; }
    if (WiFi.status() == WL_CONNECTED) { return; }
    WiFi.mode(WIFI_STA);
    uint8_t t = 0;
    for (; t < 100; t++) {
        if (WiFi.status() == WL_CONNECTED) { return; }
        // sleep(1);
        delay(10);
    }
    if (WiFi.status() != WL_CONNECTED) { setupWiFi(); }
    else { WiFiState = true; }
}

void deactivateWifi() {
    WiFi.mode(WIFI_OFF);
    WiFiState = false;
}
