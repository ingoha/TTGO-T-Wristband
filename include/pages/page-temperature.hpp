#include <Arduino.h>
#include "hal.hpp"
#include "wristband-tft.hpp"
#include "wristband-wifi.hpp"
#include "pages/page-ota.hpp"
#include "pages.hpp"

void pageTemperature(bool initialLoading);
void actionTemperature();
bool submenuTemperature(int8_t press);
