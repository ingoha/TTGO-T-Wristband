#include <Arduino.h>
#include "mpu.hpp"
#include "wristband-tft.hpp"
#include "wristband-wifi.hpp"

#include <WiFi.h>
#include <HTTPSServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <ResourceNode.hpp>

extern "C" {
#include <zforth.h>
}

using namespace httpsserver;

void pageBearing(bool initialLoading);
void actionBearing();