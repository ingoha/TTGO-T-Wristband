#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include "wristband-tft.hpp"
#include "wristband-ota.hpp"
#include "clock.hpp"
#include "pages.hpp"
#include "mpu.hpp"

void setup()
{
  Serial.begin(115200);
  setCpuFrequencyMhz(80);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
  initClock();
  tftInit();
  deactivateWifi();
  btStop();
  setupADC();
  initMPU();
  initButton();
  setupBattery();
  Serial.println("START.");
}

uint32_t last = millis();
bool bus = false;

void loop() {
  updateBatteryChargeStatus();
  if (millis() - last > 1000) {
      bus = getBusVoltage() > 4.0;
      last = millis();
  }
  if (bus) {
      status("W", WiFi.status());
      activateWifi();
      status("S", WiFi.status());
      setupOTA();
      status("O", WiFi.status());
      while (otaRunning()) usleep(10);
      refreshTimer();
  }
  handleUi();
}
