#include "pages/page-temperature.hpp"

#define OPTIONS_TEMPERATURE 6

uint32_t timeTemperature = millis();
int8_t pmenu = -1;

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
    pmenu = -1;
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
    MPU* mpu = HAL::getInstance()->getMPU();
    if (initialLoading) {
        // deactivateWifi();
        //initMPU();
        initDrawQuaternion();
        pmenu = -1;
    }
    mpu->updateDMP();
    if (millis() - timeTemperature > 300) {
        // updateMPU();
        if (pmenu >= 0) {
            drawBottomBar(getTimeout(), TFT_BLUE);
        } else {
            // refreshDrawQuaternion(getQuaternion());
            float q[7] = { 0, 0, 0, 0, 0, 0, 0 };
            mpu->getDMP(q);
            refreshDrawQuaternion(q);
        }
        timeTemperature = millis();
    }
}

void actionTemperature() {
  msgInfo("Calibrating MPU...");
  //calibrateMPU();
  HAL::getInstance()->getMPU()->calibrateMPU();
  msgInfo("MPU calibrated.");
  sleep(5);
}

bool submenuTemperature(int8_t press) {
    if (!press && pmenu < 0) { return false; }
    if (press && pmenu < 0) {
        drawOptions(options);
        pmenu = 0;
        drawMenuPointer(pmenu, OPTIONS_TEMPERATURE);
        return true;
    }
    if (press && pageActions[pmenu]) {
        pageActions[pmenu]();
        return true;
    }
    if (!options[++pmenu]) { pmenu = 0; }
    drawMenuPointer(pmenu, OPTIONS_TEMPERATURE);
    return true;
}
