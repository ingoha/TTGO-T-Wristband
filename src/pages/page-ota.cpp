#include "pages/page-ota.hpp"
#include <Arduino.h>
#include <TFT_eSPI.h>
//#include "Orbitron_Light_7.h"

PageOTA::PageOTA(HAL* h, Network* n)
{
  hal = h;
  network = n;
}

void PageOTA::draw(bool initialLoad)
{
  TFT_eSPI* tft = hal->getTFT()->getInternalTFT();
  if (initialLoad) {
    // deactivateWifi();
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextDatum(TC_DATUM);
    tft->setFreeFont(&Orbitron_Light_24);
    // msgBig("OTA");
    tft->fillScreen(TFT_BLACK);
    tft->drawString("OTA?", tft->width() / 2, 70);
  }
  if (timeout > 0 && !timeoutDrawn) {
    tft->setFreeFont(&orbitron_light7pt7b);
    tft->setTextColor(TFT_RED, TFT_BLACK);
    tft->setTextDatum(BC_DATUM);
    tft->drawString("TIMEOUT", tft->width() / 2, tft->height() - 10);
    timeoutDrawn = true;
  }
  if (timeout > 0 && millis() - timeout > 3000) {
    tft->fillScreen(TFT_BLACK);
    timeout = 0;
    timeoutDrawn = false;
  }
  tft->setFreeFont(NULL);
}

void PageOTA::action() {
  waitOta();
}

void PageOTA::waitOta() {
  unsigned long oldmilis = millis();
  unsigned long lastBar = millis();
  uint8_t lastTime = 100;
  WIFI* wifi = network->getWIFI();
  wifi->activateWifi();
  WifiOTA* ota = network->getWifiOTA();
  ota->setupOTA();
  TFT_eSPI* tft = hal->getTFT()->getInternalTFT();

  tft->setFreeFont(&orbitron_light7pt7b);
  tft->setTextDatum(TC_DATUM);
  tft->fillScreen(TFT_BLACK);
  IPAddress ip = WiFi.localIP();
  tft->drawString(String(ip[0]) + "." + String(ip[1]) + ".", tft->width() / 2, 70);
  tft->drawString(String(ip[2]) + "." + String(ip[3]), tft->width() / 2, 90);
  while (millis() - oldmilis < 30000) {
    while (ota->otaRunning()) { }
    usleep(10);
    if (millis() - lastBar > 25) {
        uint32_t pass = 100 - ((millis() - oldmilis) / 300);
        uint32_t left = 30 - ((millis() - oldmilis) / 1000);
        lastBar = millis();
        if (((uint8_t)left) != lastTime) {
            tft->setFreeFont(&Orbitron_Light_24);
            tft->setTextDatum(TC_DATUM);
            tft->drawString(String(":") + String(left < 10 ? 0 : "") + String(left), tft->width() / 2, 110);
            //hal->getTFT()->drawCommon(0, 0);
            lastTime = (uint8_t)left;
        }
    }
  }
  tft->setFreeFont(NULL);
  /*
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
  */
  wifi -> deactivateWifi();
  timeout = millis();
}
