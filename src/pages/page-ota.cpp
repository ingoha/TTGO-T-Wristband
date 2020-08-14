#include "pages/page-ota.hpp"

void pageOta(bool initialLoad)
{
  if (initialLoad)
  {
    deactivateWifi();
    msgBig("OTA");
  }
}

void waitOta()
{
  unsigned long oldmilis = millis();
  unsigned long lastBar = millis();
  uint8_t lastTime = 100;
  activateWifi();
  setupOTA();
  msgInfo2("Waiting for OTA", WiFi.localIP().toString().c_str());
  tftLandscape();
  char time[10] = "";
  while (millis() - oldmilis < 30000)
  {
    while (otaRunning()) { }
    usleep(10);
    if (millis() - lastBar > 25) {
        uint32_t pass = 100 - ((millis() - oldmilis) / 300);
        uint32_t left = 30 - ((millis() - oldmilis) / 1000);
        lastBar = millis(); drawBottomBar(pass, TFT_BLUE);
        if (((uint8_t)left) != lastTime) {
            sprintf(time, "OTA - :%02d", (uint8_t)left);
            msgInfo2(time, WiFi.localIP().toString().c_str());
            lastTime = (uint8_t)left; tftLandscape();
        }
    }
  }
  deactivateWifi();
  msgWarning("OTA Timeout");
  tftPortrait();
}
