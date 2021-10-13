#pragma once

#include <Arduino.h>
#include <EasyButton.h>
#include <vector>
#include "hal.hpp"
#include "network.hpp"

//
// this is the UI (controller) class
//
class Pages {
  private:
    int8_t page = 0;
    EasyButton* tp_button;
    uint32_t time_out = millis();
    uint16_t max_time_out = 15000;
    bool handlingAction = false;
    bool initialLoad = true;
    bool subMenu = false;
    int8_t pagesCount = 0;
    bool showCommon = true;
    int8_t menu = -1;
    int timeOut[7] = { 8, 15, 15, 60, 30, 15, 0 };
    std::vector<Pages*>* pageList;

    HAL* hal;
    Network* network;

    // what was that for ???
    //void commonLoop(void *param);

  public:
    Pages();
    void handleUi();
    void increasePage();
    void showPage();
  //  void handleSleep(bool showMsg = true);
    void initButton();
    void handleAction();
    void handlePress();
    const uint8_t getTimeout();
    void home();
    void refreshTimer();
};