#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>

class WifiOTA {
  private:
    bool otaInit  = false;
    bool otaStart = false;

  public:
    void setupOTA();
    const bool otaRunning();
    const String getOtaHostname();
};
