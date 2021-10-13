#pragma once

#include <Arduino.h>
#include "network.hpp"
#include "hal.hpp"
#include "pages.hpp"

void saveStatus(char* status);

void actionCalendar();
void pageCalendar(bool initialLoad);
void pageClock(bool initialLoad);
void pageRtc(bool initialLoad);
void actionClock();
void actionCounter();
void zeroCounter();
