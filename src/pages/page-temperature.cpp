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
    HAL::getInstance()->getTFT()->msgInfo("Test");
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
    TFT* tft = HAL::getInstance()->getTFT();
    if (initialLoading) {
        // deactivateWifi();
        //initMPU();
        tft->initDrawQuaternion();
        pmenu = -1;
    }
    mpu->updateDMP();
    if (millis() - timeTemperature > 300) {
        // updateMPU();
        if (pmenu >= 0) {
            tft->drawBottomBar(getTimeout(), TFT_BLUE);
        } else {
            // refreshDrawQuaternion(getQuaternion());
            float q[7] = { 0, 0, 0, 0, 0, 0, 0 };
            mpu->getDMP(q);
            tft->refreshDrawQuaternion(q);
        }
        timeTemperature = millis();
    }
}

void actionTemperature() {
  TFT* tft = HAL::getInstance()->getTFT();
  tft->msgInfo("Calibrating MPU...");
  //calibrateMPU();
  HAL::getInstance()->getMPU()->calibrateMPU();
  tft->msgInfo("MPU calibrated.");
  sleep(5);
}

bool submenuTemperature(int8_t press) {
    TFT* tft = HAL::getInstance()->getTFT();

    if (!press && pmenu < 0) { return false; }
    if (press && pmenu < 0) {
        tft->drawOptions(options);
        pmenu = 0;
        tft->drawMenuPointer(pmenu, OPTIONS_TEMPERATURE);
        return true;
    }
    if (press && pageActions[pmenu]) {
        pageActions[pmenu]();
        return true;
    }
    if (!options[++pmenu]) { pmenu = 0; }
    tft->drawMenuPointer(pmenu, OPTIONS_TEMPERATURE);
    return true;
}
