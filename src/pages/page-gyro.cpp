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

const uint16_t PageGyro::timeout() { return 30;}

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
    mpu->startFilter();
    TFT_eSPI* tft = hal->getTFT()->getInternalTFT();
    if (initialLoading) {
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
        pmenu = -1;
    }
    else {
        if (millis() - timeTemperature > 300) {
        mpu->update();
        if (pmenu >= 0) {
            //tft->drawBottomBar(getTimeout(), TFT_BLUE);
        } 
        else {
            //float q[7] = { 0, 0, 0, 0, 0, 0, 0 };
            //mpu->gagewatchRead(q);
            //mpu->getDMP(q);
            //tft->refreshDrawQuaternion(q);
            tft->setTextDatum(TL_DATUM);
            tft->setTextColor(TFT_ORANGE, TFT_BLACK);
            char q0[8] = "";
            char qx[8] = "";
            char qy[8] = "";
            char qz[8] = "";
            char syaw[8] = "";
            char spitch[8] = "";
            char sroll[8] = "";
            MPU::xyz mag = mpu->getMag();
            sprintf(qx, "%5.1f", mag.x);
            sprintf(qy, "%5.1f", mag.y);
            sprintf(qz, "%5.1f", mag.z);

            sprintf(syaw, "%5.1f", mpu->getYaw());
            sprintf(spitch, "%5.1f", mpu->getPitch());
            sprintf(sroll, "%5.1f", mpu->getRoll());
            tft->drawString(q0, 40, 10, 2);
            tft->drawString(qx, 40, 30, 2);
            tft->drawString(qy, 40, 50, 2);
            tft->drawString(qz, 40, 70, 2);
            tft->drawString(syaw, 40, 90, 2);
            tft->drawString(spitch, 40, 110, 2);
            tft->drawString(sroll, 40, 130, 2);

           char buf[100];
           sprintf(buf, "yaw: %2f, pitch: %2f, roll %2f", 
                        mpu->getYaw(), mpu->getPitch(), mpu->getRoll());
           Serial.println(buf);
        }
        timeTemperature = millis();
    }
    }
}

void PageGyro::action() {
  TFT* tft = hal->getTFT();
  tft->msgInfo("Calibrating MPU...");
  //hal->getMPU()->calibrateMPU();
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
