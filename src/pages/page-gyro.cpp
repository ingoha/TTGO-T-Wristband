#include "pages/page-gyro.hpp"

// cf. https://www.seeedstudio.com/blog/2020/06/17/how-to-use-an-mpu9250-accelerometer-and-gyroscope-with-arduino/

#define OPTIONS_TEMPERATURE 6

const char *options[OPTIONS_TEMPERATURE + 1] = {
    "OTA",
    "Kalibruj",
    "Test",
    "Start",
    "Zeruj",
    "WSTECZ",
    NULL
};
/*
void menuActionTest() {
    HAL::getInstance()->getTFT()->msgInfo("Test");
    delay(2000);
    show(true);
}

void menuBack() {
    //pmenu = -1;
    show(true);
}
*/

/*
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
*/

void PageGyro::draw(bool initialLoading) {
    MPU* mpu = hal->getMPU();
    TFT* tft = hal->getTFT();
    if (initialLoading) {
        // deactivateWifi();
        tft->initDrawQuaternion();
        pmenu = -1;
    }
    mpu->updateDMP();
    if (millis() - timeTemperature > 300) {
        // updateMPU();
        if (pmenu >= 0) {
            //tft->drawBottomBar(getTimeout(), TFT_BLUE);
        } 
        else {
            //tft->refreshDrawQuaternion(mpu->getQuaternion());
            float q[7] = { 0, 0, 0, 0, 0, 0, 0 };
            mpu->gagewatchRead(q);
            mpu->getDMP(q);
            tft->refreshDrawQuaternion(q);
        }
        timeTemperature = millis();
    }
}

void PageGyro::action() {
  TFT* tft = hal->getTFT();
  tft->msgInfo("Calibrating MPU...");
  hal->getMPU()->calibrateMPU();
  tft->msgInfo("MPU calibrated.");
  sleep(5);
}

bool PageGyro::submenu(int8_t press) {
    TFT* tft = hal->getTFT();

    if (!press && pmenu < 0) { return false; }
    if (press && pmenu < 0) {
        tft->drawOptions(options);
        pmenu = 0;
        tft->drawMenuPointer(pmenu, OPTIONS_TEMPERATURE);
        return true;
    }
    /*
    if (press && pageActions[pmenu]) {
        pageActions[pmenu]();
        return true;
    }
    */
    if (!options[++pmenu]) { pmenu = 0; }
    tft->drawMenuPointer(pmenu, OPTIONS_TEMPERATURE);
    return true;
}
