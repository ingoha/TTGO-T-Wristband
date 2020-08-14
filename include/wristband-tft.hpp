#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include <hal.hpp>

#define SEG7_BACKGROUND 0x0821

void tftInit();
void wifiManagerAdvice(const char *ap_name);
void drawProgressBar(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint8_t percentage, uint16_t frameColor, uint16_t barColor);
void updatingText();
void msgError(const char *message);
void msgWarning(const char *message);
void msgSuccess(const char *message);
void msgInfo(const char *message);
void msgInfo2(const char * msgA, const char* msgB);
void tftSleep(bool showMsg);
void msgBig(const char *message);
void msg(const char *message, uint16_t color);
void msg2(const char *msgA, const char *msgB, uint16_t color);
void displayDate(const uint8_t day, const uint8_t month, const uint16_t year, bool utc);
uint16_t displayHour(const uint8_t hour, const uint8_t minute, bool utc);
uint16_t displayColon(uint16_t x, bool color, bool utc);
void clearScreen();
void drawBattery(float voltage, uint8_t percentage, bool charging);
void initDrawBearing();
void refreshDrawBearing(int16_t bearing);
void initDrawTemperature();
void refreshDrawTemperature(float temperature);
void drawBottomBar(uint8_t percent, uint16_t color);
void tftLandscape();
void tftPortrait();
void displayBatteryValue(float voltage, uint8_t percent, bool charging);
void initDrawQuaternion();
void refreshDrawQuaternion(const float *q);
void displayAppointments();
void drawOptions(const char *options[], uint8_t n);
void drawMenuPointer(int8_t n, uint8_t max);
void drawStatus(char symbol, bool status, uint8_t pos);
void displayCounter(float counter, float add, float percent);