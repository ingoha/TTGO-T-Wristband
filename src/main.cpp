#include "clock.hpp"
#include "mpu.hpp"
#include "pages.hpp"
#include "wristband-ota.hpp"
#include "wristband-tft.hpp"
#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>

uint32_t last = millis();
bool bus = false;

void checkBus(void *param) {
  for (;;) {
    bus = getBusVoltage() > 4.0;
    updateBatteryChargeStatus();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  setCpuFrequencyMhz(80);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
  initClock();
  initWiFi();
  tftInit();
  deactivateWifi();
  btStop();
  setupADC();
  initMPU();
  initButton();
  setupBattery();
  Serial.println("START.");

  xTaskCreate(checkBus, "CheckBusVoltage", 1024, NULL, 1, NULL);
}

void loop() {
  if (bus) {
    activateWifi();
    setupOTA();
    while (otaRunning())
      usleep(10);
    refreshTimer();
  }
  handleUi();
}
