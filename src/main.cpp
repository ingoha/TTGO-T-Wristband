#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include "pages.hpp"
#include "hal.hpp"
#include "network.hpp"
#include "pins.hpp"

//#define DEBUG

uint32_t last = millis();
bool bus = false;

void checkBus(void *param) {
  Battery* bat = HAL::getInstance()->getBattery();
  for (;;) {
    bus = bat->getBusVoltage() > 4.0;
    bat->updateBatteryChargeStatus();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

Pages* pages;

void debug(String msg)
{
  #ifdef DEBUG
  Serial.println(msg);
  #endif
}

void setup() {
  Serial.begin(115200);
  debug("Startup...");
  setCpuFrequencyMhz(80);
  debug("Setup Wire...");
  // FIXME Wire must be global for MPU library :-/
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
  debug("Setup HAL...");
  HAL* hal = HAL::getInstance();

  debug("Setup WIFI...");
  WIFI* wifi = Network::getInstance()->getWIFI();
  wifi->setupWiFi();
  debug("Init WIFI...");
  wifi->initWiFi();
  debug("Deactivate WIFI...");
  wifi->deactivateWifi();
  debug("Stop BT...");
  btStop();

  debug("Setup UI...");
  pages = new Pages();
  Serial.println("START.");

  xTaskCreate(checkBus, "CheckBusVoltage", 1024, NULL, 1, NULL);
}

void loop() {
  debug("loop");
  if (bus) {
    Network::getInstance()->getWIFI()->activateWifi();
    Network::getInstance()->getWifiOTA()->setupOTA();
    while (Network::getInstance()->getWifiOTA()->otaRunning())
      usleep(10);
    pages->refreshTimer();
  }
  pages->handleUi();
}
