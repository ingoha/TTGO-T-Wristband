#include <Arduino.h>
#include "wristband-ota.hpp"
#include "network.hpp"

#include <TFT_eSPI.h>

void pageOta(bool initialLoad);
void waitOta();