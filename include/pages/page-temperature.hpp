#include <Arduino.h>
#include "mpu.hpp"
#include "wristband-tft.hpp"
#include "wristband-wifi.hpp"
#include "pages/page-ota.hpp"
#include "pages.hpp"

void pageTemperature(bool initialLoading);
void actionTemperature();
bool submenuTemperature(bool press);

