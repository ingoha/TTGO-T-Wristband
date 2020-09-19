#include <Arduino.h>
#include "wristband-tft.hpp"
#include "wristband-ota.hpp"
#include "wristband-wifi.hpp"

#include <TFT_eSPI.h>

void pageOta(bool initialLoad);
void waitOta();