#include "wristband-tft.hpp"
#include "hal.hpp"
#include "network.hpp"

TFT_eSPI* TFT::getInternalTFT() {
  return tft;
}

TFT::TFT() {
  landscape = true;
  tft = new TFT_eSPI();
  tft->init();
  tftPortrait();
  tft->setSwapBytes(true);
  tft->fillScreen(TFT_BLACK);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, 185);
}

void TFT::wifiManagerAdvice(const char *ap_name) {
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(TFT_WHITE);
  tft->drawString("Connect hotspot name ", 20, tft->height() / 2 - 20);
  tft->drawString("configure wrist", 35, tft->height() / 2 + 20);
  tft->setTextColor(TFT_GREEN);
  tft->setTextDatum(MC_DATUM);
  tft->drawString(ap_name, tft->width() / 2, tft->height() / 2);
}

void TFT::drawProgressBar(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h,
                     uint8_t percentage, uint16_t frameColor,
                     uint16_t barColor) {
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextDatum(TC_DATUM);
  tft->setTextPadding(tft->textWidth(" 888% "));
  tft->drawString(String(percentage) + "%", 145, 35);
  if (percentage == 0) {
    tft->fillRoundRect(x0, y0, w, h, 3, TFT_BLACK);
  }
  uint8_t margin = 2;
  uint16_t barHeight = h - 2 * margin;
  uint16_t barWidth = w - 2 * margin;
  tft->drawRoundRect(x0, y0, w, h, 3, frameColor);
  tft->fillRect(x0 + margin, y0 + margin, barWidth * percentage / 100.0,
               barHeight, barColor);
}

void TFT::updatingText() {
  // tftLandscape();
  tftPortrait();
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  // tft->drawString("Updating...", tft->width() / 2 - 20, 55);
}

void TFT::drawOTA(int progress) {
  // tftPortrait();
  static int last = -1;
  if (progress == last && progress > 0) {
    return;
  }
  if (progress % 5 == 0) {
    drawCommon(0, 0);
  }
  last = progress;
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setFreeFont(&Orbitron_Light_24);
  tft->setTextDatum(TC_DATUM);
  if (progress % 5 == 0) {
    // tft->fillScreen(TFT_BLACK);
    tft->drawRect(0, 60, tft->width(), tft->height() - 60, TFT_BLACK);
    tft->drawString("OTA", tft->width() / 2, 70);
  }
  tft->setTextPadding(tft->textWidth(" 888% "));
  tft->drawString(String(progress) + "%", tft->width() / 2, 100);
  tft->drawLine(0, tft->height() - 2, tft->width(), tft->height() - 2, TFT_WHITE);
  tft->drawLine(0, tft->height() - 1, tft->width() * progress / 100.0,
               tft->height() - 1, TFT_CYAN);
  tft->setFreeFont(NULL);
}

void TFT::msgBig(const char *message) {
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(TFT_CYAN);
  tft->setTextDatum(MC_DATUM);
  tft->drawString(message, tft->width() / 2, tft->height() / 2, 4);
}

void TFT::msgError(const char *message) { msg(message, TFT_RED); }

void TFT::msgWarning(const char *message) { msg(message, TFT_ORANGE); }

void TFT::msgSuccess(const char *message) { msg(message, TFT_GREEN); }

void TFT::msgInfo(const char *message) { msg(message, TFT_CYAN); }

void TFT::msgInfo2(const char *msgA, const char *msgB) {
  msg2(msgA, msgB, TFT_CYAN);
}

void TFT::msg(const char *message, uint16_t color) {
  tftLandscape();
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(color);
  tft->setTextDatum(MC_DATUM);
  tft->drawString(message, tft->width() / 2, tft->height() / 2, 2);
  tftPortrait();
}

void TFT::msg2(const char *msgA, const char *msgB, uint16_t color) {
  tftLandscape();
  tft->fillScreen(TFT_BLACK);
  tft->setTextColor(color);
  tft->setTextDatum(MC_DATUM);
  tft->drawString(msgA, tft->width() / 2, tft->height() / 3, 2);
  tft->drawString(msgB, tft->width() / 2, tft->height() * 2 / 3, 2);
  tftPortrait();
}

void TFT::sleep(bool showMsg) {
  if (showMsg) {
    tft->fillScreen(TFT_BLACK);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("Press again to wake up", tft->width() / 2, tft->height() / 2);
    delay(2000);
  }
  tft->fillScreen(TFT_BLACK);
  tft->writecommand(ST7735_SWRESET);
  delay(100);
  tft->writecommand(ST7735_SLPIN);
  delay(150);
  tft->writecommand(ST7735_DISPOFF);
}

void TFT::clearScreen() {
  tft->fillScreen(TFT_BLACK);
  tft->setCursor(0, 0);
}

void TFT::displayDate(const uint8_t day, const uint8_t month, const uint16_t year,
                 bool utc) {
  char date[11] = " ";
  sprintf(date, "%02u.%02u.%u", day, month, year);
  tft->setTextDatum(TL_DATUM);
  // tft->setTextColor(TFT_GREEN, TFT_BLACK);
  tft->setTextColor(0xFBE0, TFT_BLACK);
  tft->setCursor(6, 108);
  tft->print(date);
  if (utc) {
    tft->print("U");
  }
}

uint16_t TFT::displayHour(const uint8_t hour, const uint8_t minute, bool utc) {
  uint8_t xpos = 6;
  uint8_t yposa = 6;
  uint8_t yposb = 54;
  uint16_t colonX = 0;
  tft->setTextDatum(TL_DATUM);
  tft->setTextColor(SEG7_BACKGROUND, TFT_BLACK);
  tft->drawString("88", xpos, yposa, 7);
  tft->drawString("88", xpos, yposb, 7);
  tft->setTextColor(utc ? TFT_GREENYELLOW : 0xFBE0);
  if (hour < 10) {
    xpos += tft->drawChar('0', xpos, yposa, 7);
  }
  xpos += tft->drawNumber(hour, xpos, yposa, 7);
  colonX = xpos;
  xpos += displayColon(xpos, true, utc);
  xpos = 6;
  tft->setTextColor(utc ? TFT_GREENYELLOW : 0xFBE0);
  if (minute < 10) {
    xpos += tft->drawChar('0', xpos, yposb, 7);
  }
  tft->drawNumber(minute, xpos, yposb, 7);
  return colonX;
}

uint16_t TFT::displayColon(uint16_t x, bool color, bool utc) {
  if (color) {
    tft->setTextColor(0x0821);
  } else {
    tft->setTextColor(utc ? TFT_GREENYELLOW : 0xFBE0);
  }
  return tft->drawChar(':', x, 6, 7);
}

void TFT::drawBattery(float voltage, uint8_t percentage, bool charging) {
  char voltageString[5] = " ";
  uint16_t originx = 70;
  uint16_t originy = 20;
  uint16_t width = 80;
  uint16_t height = 40;
  uint16_t tabheight = 15;
  uint16_t tabwidth = 5;
  uint8_t margin = 2;
  uint16_t barHeight = height - 2 * margin;
  uint16_t barWidth = width - 2 * margin;
  sprintf(voltageString, "%2.2f", voltage);

  tft->fillScreen(TFT_BLACK);

  tftLandscape();
  if (percentage == 0) {
    tft->fillRoundRect(originx, originy, width, height, 3, TFT_BLACK);
  }
  tft->fillRoundRect(originx - tabwidth + 1, (height - tabheight) / 2 + originy,
                    tabwidth, tabheight, 1, TFT_WHITE);
  tft->drawRoundRect(originx, originy, width, height, 3, TFT_WHITE);
  uint8_t barFill = barWidth * percentage / 100.0;
  tft->fillRect(originx + margin + (barWidth - barFill), originy + margin,
               barFill, barHeight, TFT_DARKGREEN);

  tft->setTextColor(TFT_WHITE);
  tft->setTextDatum(MC_DATUM);
  tft->setTextPadding(tft->textWidth(" 888% ", 2));
  width = tft->width();
  originx = 40;
  height = 0;
  originy = 40;
  tft->drawString(String(percentage) + "%", width / 2 + originx,
                 height / 2 + originy, 2);

  tftPortrait();
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextDatum(BC_DATUM);
  String voltageInfo = String(voltageString) + "V";
  if (charging) {
    voltageInfo += " CHRG";
  }
  tft->drawString(voltageInfo, tft->width() / 2, tft->height());
}

void TFT::initDrawBearing() {
  tft->fillScreen(TFT_BLACK);
  tft->setTextDatum(MC_DATUM);
  tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  tft->drawString("---", tft->width() / 2, 40, 4);
  tft->drawCircle(40, 40, 35, TFT_WHITE);
}

void TFT::refreshDrawBearing(int16_t bearing) {
  char bearingText[5] = "---\0";
  if (bearing >= 0) {
    sprintf(bearingText, "%03d", bearing);
  }
  tft->setTextDatum(MC_DATUM);
  tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  tft->fillCircle(40, 40, 34, TFT_BLACK);
  tft->drawString(bearingText, tft->width() / 2, 40, 4);
  if (bearing >= 0) {
    double bs = sin((360 - bearing) * PI / 180.0);
    double bc = cos((360 - bearing) * PI / 180.0);
    uint8_t iy = 40 + 30 * bs;
    uint8_t ix = 40 + 30 * bc;
    uint8_t oy = 40 + 34 * bs;
    uint8_t ox = 40 + 34 * bc;
    tft->drawLine(ix, iy, ox, oy, TFT_WHITE);
  }
}

void TFT::initDrawTemperature() {
  // tft->fillScreen(TFT_BLACK);
  tft->setTextDatum(BR_DATUM);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->drawString("C", tft->width() - 5, tft->height(), 2);
  tft->drawCircle(tft->width() - 18, tft->height() - 12, 2, TFT_WHITE);
}

void TFT::refreshDrawTemperature(float temperature) {
  char temperatureText[8] = " ";
  sprintf(temperatureText, "%.1f", temperature);
  tft->setTextDatum(TC_DATUM);
  tft->setTextColor(TFT_RED, TFT_BLACK);
  tft->drawString(temperatureText, tft->width() / 2, 100, 4);
}

void TFT::drawBottomBar(uint8_t percent, uint16_t color) {
  if (percent > 100) {
    percent = 100;
  }
  if (color == 0) {
    color = TFT_GREEN;
    if (percent <= 50) {
      color = TFT_YELLOW;
    }
    if (percent <= 10) {
      color = TFT_ORANGE;
    }
    if (percent <= 5) {
      color = TFT_RED;
    }
  }
  uint8_t posx = landscape ? 159 : 79;
  uint8_t posy = landscape ? 77 : 157;
  uint16_t len = posx - 3;
  len *= percent;
  len /= 100;
  tft->drawLine(1, posy, 1, posy + 2, TFT_WHITE);
  tft->fillRect(2, posy, (posx - 3) + 2, 3, TFT_BLACK);
  tft->fillRect(2, posy, len + 2, 3, color);
  tft->drawLine(posx, posy, posx, posy + 2, TFT_WHITE);
}

void TFT::tftLandscape() {
  tft->setRotation(1);
  landscape = true;
}
void TFT::tftPortrait() {
  tft->setRotation(0);
  landscape = false;
}

void TFT::status(const char *msg, int state) {
  tft->setFreeFont(&TomThumb);
  tft->setTextDatum(TC_DATUM);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextPadding(tft->width());
  if (state < 0) {
    tft->drawString(msg, tft->width() / 2, tft->height() - 6);
  } else {
    tft->drawString(String(msg) + " " + String(state) + "  ", tft->width() / 2,
                   tft->height() - 6);
  }
}

void TFT::displayBatteryValue(float voltage, uint8_t percent, bool charging) {
  char str[14] = "";
  sprintf(str, "%2.2fV %3d%% %s", voltage, percent, charging ? "+" : " ");
  tft->setTextDatum(TL_DATUM);
  tft->setTextColor(0xFBE0, TFT_BLACK);
  tft->drawString(str, 6, 145);
}

void TFT::initDrawQuaternion() {
  tft->fillScreen(TFT_BLACK);
  tft->setTextDatum(TL_DATUM);
  tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  tft->drawString("q0 = ", 6, 10, 2);
  tft->drawString("qx = ", 6, 30, 2);
  tft->drawString("qy = ", 6, 50, 2);
  tft->drawString("qz = ", 6, 70, 2);
  tft->drawString("Y = ", 6, 90, 2);
  tft->drawString("P = ", 6, 110, 2);
  tft->drawString("R = ", 6, 130, 2);
}

void TFT::refreshDrawQuaternion(const float *q) {
  tft->setTextDatum(TL_DATUM);
  tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  char q0[8] = "";
  char qx[8] = "";
  char qy[8] = "";
  char qz[8] = "";
  char syaw[8] = "";
  char spitch[8] = "";
  char sroll[8] = "";
  sprintf(q0, "%6.3f", q[0]);
  sprintf(qx, "%6.3f", q[1]);
  sprintf(qy, "%6.3f", q[2]);
  sprintf(qz, "%6.3f", q[3]);
  float yaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]),
                    q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
  float pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
  float roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]),
                     q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
  pitch *= 180.0f / PI;
  yaw *= 180.0f / PI;
  roll -= (4 + (40 / 60)); // Declination
  roll *= 180.0f / PI;
  sprintf(syaw, "%5.1f", yaw);
  sprintf(spitch, "%5.1f", pitch);
  sprintf(sroll, "%5.1f", roll);
  tft->drawString(q0, 40, 10, 2);
  tft->drawString(qx, 40, 30, 2);
  tft->drawString(qy, 40, 50, 2);
  tft->drawString(qz, 40, 70, 2);
  tft->drawString(syaw, 40, 90, 2);
  tft->drawString(spitch, 40, 110, 2);
  tft->drawString(sroll, 40, 130, 2);
}

uint8_t TFT::drawOptions(const char *options[]) {
  tft->fillScreen(TFT_BLACK);
  tft->setTextDatum(ML_DATUM);
  tft->setFreeFont(&Orbitron_Light5pt7b);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  uint8_t i = 0;
  for (i = 0; options[i]; i++) {
    if (options[i]) {
      tft->drawString(options[i], 20, 55 + (i + 1) * 16);
    }
  }
  return i + 1;
}

void TFT::drawMenuPointer(int8_t n, bool right) {
  uint8_t width = 10;
  uint8_t from = 3;
  tft->fillRect(from, 65, width, 100, TFT_BLACK);
  uint8_t y = (n + 1) * 16 + 55;
  uint8_t x = width + from - 1;
  if (right) {
    tft->fillTriangle(from, y - 5, x, y, from, y + 5, TFT_WHITE);
  } else {
    tft->fillTriangle(x, y - 5, from, y, x, y + 5, TFT_WHITE);
  }
}

void TFT::drawStatus(char symbol, bool status, uint8_t pos) {
  char str[2];
  str[0] = symbol;
  str[1] = '\0';
  tft->setTextDatum(TC_DATUM);
  tft->setTextColor(status ? TFT_ORANGE : TFT_DARKGREY, TFT_BLACK);
  tft->drawString(str, pos, 135, 1);
}

void TFT::displayCounter(float counter, float add, float percent) {
  char str[24] = "0.00";
  char per[24] = "0.00";
  sprintf(str, "%.1f +%.1f", counter, add);
  sprintf(per, "%.3f%%", percent);
  tft->setTextDatum(TC_DATUM);
  // tft->setTextColor(TFT_GREEN, TFT_BLACK);
  tft->setTextColor(TFT_ORANGE, TFT_BLACK);
  tft->drawString(str, tft->width() / 2, 120, 2);
  tft->drawString(per, tft->width() / 2, 140, 2);
}

void TFT::readRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t *data) {
  tft->readRectRGB(x, y, w, h, data);
}

bool TFT::drawCommon(uint8_t page, uint8_t pages) {
  bool cleared = false;
  static int8_t oldMinute = -1;
  RTC_Date current;
  float voltage, bvoltage;
  current = HAL::getInstance()->getClock()->getClockTime();
  if (current.minute != oldMinute) {
    tft->fillScreen(TFT_BLACK);
    cleared = true;
  }
  oldMinute = current.minute;
  // current = getClockTime();
  tft->setTextPadding(0);
  tft->setFreeFont(&Orbitron_Light_24);
  /*
  if (!page) {
    tft->setTextColor(TFT_RED, TFT_BLACK);
    tft->setTextDatum(TC_DATUM);
    tft->drawString(String(page).c_str(), tft->width() / 2, tft->height() - 50);
  }
  */
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextDatum(TL_DATUM);
  tft->drawString(
      String(current.hour < 10 ? "0" : "") + String(current.hour) + ":", 0, 0);
  tft->drawString(
      String(current.minute < 10 ? "0" : "") + String(current.minute), 0, 25);
  tft->setFreeFont(&orbitron_light7pt7b);
  tft->setTextDatum(TR_DATUM);
  tft->drawString(String(current.day) + ".", tft->width() - 1, 0);
  tft->drawString(String(current.month < 10 ? "0" : "") + String(current.month) +
                     ".",
                 tft->width() - 1, 14);
  tft->drawString(HAL::getInstance()->getClock()->getClockDayName(), tft->width() - 1, 28);
  /* if (lastError[0] != 0) {
    tft->setTextColor(TFT_RED, TFT_BLACK);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextDatum(TC_DATUM);
    tft->drawString(lastError, tft->width(), 100);
  } */
  tft->setFreeFont(&TomThumb);
  tft->setTextDatum(TR_DATUM);
  tft->drawString((Network::getInstance()->getWIFI()->WiFiConnected() ? "W " : "") + String(current.year),
                 tft->width(), 45);
  tft->setTextDatum(TR_DATUM);
  Battery* bat = HAL::getInstance()->getBattery();
  voltage = bat->getVoltage();
  bvoltage = bat->getBusVoltage();
  uint8_t percent = bat->calcPercentage(voltage);
  tft->drawString(String(percent) + "% " + String(bvoltage, 2) + "V " +
                     String(voltage, 2) + "V" + (bat->isCharging() ? "+" : ""),
                 tft->width() + 3, 52);
  tft->setTextDatum(TC_DATUM);
  // tft->drawString(String(percent) + "%", tft->width() / 2, 52);
  tft->setTextDatum(TL_DATUM);
  tft->drawString(String(page) + "/" + String(pages), 0, 52);
  tft->setFreeFont(NULL);
  tft->drawLine(0, 58, tft->width(), 58, TFT_GREY);
  uint16_t color = TFT_GREEN;
  if (percent <= 50) {
    color = TFT_YELLOW;
  }
  if (percent <= 10) {
    color = TFT_ORANGE;
  }
  if (percent <= 5) {
    color = TFT_RED;
  }
  tft->drawLine(0, 58, tft->width() * percent / 100.0, 58, color);
  if (pages > 0) {
    uint8_t pw = tft->width() / (pages + 1);
    uint8_t pr = 0;
    if (pages > 1) {
      pr = (tft->width() - (pw * pages)) / (pages - 1);
    }
    for (uint8_t i = 0; i < pages; i++) {
      uint8_t px = i * pw + i * pr;
      tft->drawLine(px, 60, px + pw, 60, i == page ? TFT_CYAN : TFT_WHITE);
    }
  }
  return cleared;
}
