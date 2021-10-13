#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include "pages.hpp"
#include "hal.hpp"
#include "network.hpp"
#include "pins.hpp"

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

void setup() {
  Serial.begin(115200);
  setCpuFrequencyMhz(80);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
  HAL* hal = HAL::getInstance();
  WIFI* wifi = Network::getInstance()->getWIFI();
  pages = new Pages();
  //initClock();
  hal->getClock();
  wifi->initWiFi();
  //tftInit();
  hal->getTFT();
  wifi->deactivateWifi();
  btStop();
  // FIXME Das alles in eine HAL.init???
  //setupADC();
  hal->getBattery();
  //initMPU();
  hal->getMPU();
  pages->initButton();
  //setupBattery();
  Serial.println("START.");

  xTaskCreate(checkBus, "CheckBusVoltage", 1024, NULL, 1, NULL);
}

void loop() {
  if (bus) {
    Network::getInstance()->getWIFI()->activateWifi();
    Network::getInstance()->getWifiOTA()->setupOTA();
    while (Network::getInstance()->getWifiOTA()->otaRunning())
      usleep(10);
    pages->refreshTimer();
  }
  pages->handleUi();
}
