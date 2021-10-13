#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "wristband-tft.hpp"
#include "pages.hpp"

class WifiOTA {
  private:
    bool otaInit  = false;
    bool otaStart = false;

  public:
    void setupOTA();
    const bool otaRunning();
    const String getOtaHostname();
};
