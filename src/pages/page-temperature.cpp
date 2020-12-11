#include "pages/page-temperature.hpp"

#define OPTIONS_TEMPERATURE 6

uint32_t timeTemperature = millis();
int8_t menu = -1;

const char *options[] = {
    "OTA",
    "Kalibruj",
    "Test",
    "Start",
    "Zeruj",
    "WSTECZ",
    NULL
};

void menuActionTest() {
    msgInfo("Test");
    delay(2000);
    pageTemperature(true);
}

void menuBack() {
    menu = -1;
    pageTemperature(true);
}


typedef void(*Action)();
Action pageActions[] = {
    waitOta,
    actionTemperature,
    menuActionTest,
    home,
    zeroCounter,
    menuBack,
    NULL,
};

void pageTemperature(bool initialLoading) {
    if (initialLoading) {
        // deactivateWifi();
        initMPU();
        initDrawQuaternion();
        menu = -1;
    }
    updateDMP();
    if (millis() - timeTemperature > 300) {
        // updateMPU();
        if (menu >= 0) {
            drawBottomBar(getTimeout(), TFT_BLUE);
        } else {
            // refreshDrawQuaternion(getQuaternion());
            float q[7] = { 0, 0, 0, 0, 0, 0, 0 };
            getDMP(q);
            refreshDrawQuaternion(q);
        }
        timeTemperature = millis();
    }
}

void actionTemperature() {
  msgInfo("Calibrating MPU...");
  calibrateMPU();
  msgInfo("MPU calibrated.");
  sleep(5);
}

bool submenuTemperature(bool press) {
    if (!press && menu < 0) { return false; }
    if (press && menu < 0) {
        drawOptions(options, OPTIONS_TEMPERATURE);
        menu = 0;
        drawMenuPointer(menu, OPTIONS_TEMPERATURE);
        return true;
    }
    if (press && pageActions[menu]) {
        pageActions[menu]();
        return true;
    }
    if (!options[++menu]) { menu = 0; }
    drawMenuPointer(menu, OPTIONS_TEMPERATURE);
    return true;
}
