#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include "wristband-tft.hpp"
#include "pages.hpp"

void setupOTA();
bool otaRunning();
String getOtaHostname();