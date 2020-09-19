#pragma once

#include <Arduino.h>
#include "clock.hpp"
#include "ntp.hpp"
#include "battery.hpp"
#include "wristband-tft.hpp"
#include "wristband-wifi.hpp"
#include "pages.hpp"

void showClock(uint8_t page, uint8_t pages);

void pageClock(bool initialLoad);
void pageRtc(bool initialLoad);
void actionClock();
void actionCounter();
void zeroCounter();