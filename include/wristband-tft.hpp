#ifndef TFT_H
#define TFT_H

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "fonts/Orbitron_Light_5.h"
#include "fonts/Orbitron_Light_6.h"
#include "fonts/Orbitron_Light_7.h"

#define TPOS(h,m) (4 + h * 3 + m / 20)
#define SPAD(d) ((d < 10 ? "0" : "") + String(d))

class TFT {
	private:
		int SEG7_BACKGROUND = 0x0821;
		int TFT_GREY = 0xBDF7;

		TFT_eSPI* tft;

		bool landscape;

	public:
		//void tftInit();
		TFT();
		void wifiManagerAdvice(const char *ap_name);
		void drawProgressBar(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h,
                     uint8_t percentage, uint16_t frameColor,
                     uint16_t barColor);
		void drawOTA(int progress);
		void updatingText();
		void status(/* MSG */ const char *msg, // message
        int state);
		void msgError(const char *message);
		void msgWarning(const char *message);
		void msgSuccess(const char *message);
		void msgInfo(const char *message);
		void msgInfo2(const char *msgA, const char *msgB);
		void sleep(bool showMsg);
		void msgBig(const char *message);
		void msg(const char *message, uint16_t color);
		void msg2(const char *msgA, const char *msgB, uint16_t color);
		void displayDate(const uint8_t day, const uint8_t month, const uint16_t year,
                 bool utc);
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
		uint8_t drawOptions(const char *options[]);
		void drawMenuPointer(int8_t n, bool right);
		void drawStatus(char symbol, bool status, uint8_t pos);
		void displayCounter(float counter, float add, float percent);
		void readRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t *data);
		bool drawCommon(uint8_t page, uint8_t pages);

		// q&d for OTA
		TFT_eSPI* getInternalTFT();
};
#endif