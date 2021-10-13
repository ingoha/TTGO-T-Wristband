#include "wristband-ota.hpp"

void WifiOTA::setupOTA()
{
    if (otaInit) { return; }
  ArduinoOTA.setHostname("T-Wristband");
  ArduinoOTA.setPassword("wristbandpass");

  ArduinoOTA.onStart([this]() {
              String type;
              if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
              else // U_SPIFFS
                type = "filesystem";
              Serial.println("Start updating " + type);
              this->otaStart = true;
              HAL::getInstance()->getTFT()->updatingText();
            })
      .onEnd([]() {
        Serial.println("\nEnd");
        delay(500);
        esp_restart();
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        int percentage = (progress / (total / 100));
        // drawProgressBar(10, 30, 120, 15, percentage, TFT_WHITE, TFT_BLUE);
        HAL::getInstance()->getTFT()->drawOTA(percentage);
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      })
      .onError([this](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) {
          Serial.println("Begin Failed");
          // lastError = "EOTA: BEGIN";
        } else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
        HAL::getInstance()->getTFT()->msgWarning("UPDATE FAILED!");
        delay(3000);
        this->otaStart = false;
        home();
      });

  ArduinoOTA.begin();
  otaInit = true;
}

const bool WifiOTA::otaRunning() {
    if (!otaInit) { return false; }
    ArduinoOTA.handle();
    return otaStart;
}
