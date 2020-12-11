#include <Arduino.h>
#include <EasyButton.h>
#include "pages.hpp"

int8_t page = 0;
EasyButton tp_button(TP_PIN_PIN, 80, true, false);
uint32_t time_out = millis();
uint16_t max_time_out = 15000;
bool handlingAction = false;
bool initialLoad = true;
bool subMenu = false;
int8_t pagesCount = 0;

typedef void(*Page)(bool);
Page pages[] = {
    NULL,
    pageClock,
    pageRtc,
    pageBattery,
    pageBearing,
    pageTemperature,
    pageOta,
    handleSleep,
    NULL,
    NULL,
    NULL
};

typedef void(*Action)();
Action actions[] = {
    NULL,
    actionClock,
    actionCounter,
    waitOta,
    actionBearing,
    NULL,
    waitOta,
    NULL,
    NULL
};

typedef bool(*Submenu)(bool);
Submenu submenus[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    submenuTemperature,
    NULL,
    NULL,
    NULL
};

int timeOut[] = { 8, 15, 15, 60, 30, 15, 0 };

void initButton() {
  pinMode(TP_PWR_PIN, PULLUP);
  digitalWrite(TP_PWR_PIN, HIGH);
  tp_button.begin();
  tp_button.onPressedFor(1000, handleAction);
  tp_button.onPressed(handlePress);
  page = 0;
  showPage();
  for (pagesCount = 0; (pages[pagesCount] || pages[pagesCount+1]) && pagesCount < 10; pagesCount++) {}
}

void refreshTimer() {
    time_out = millis();
}

void handleUi() {
    if (!isCharging()) { digitalWrite(LED_PIN, digitalRead(TP_PIN_PIN)); }
    /*
    tp_button.read();
    if (getBusVoltage() > 4.0) {
        if (!handlingAction) { showPage(); }
        return;
    }
    */
    if (millis() - time_out > max_time_out && !handlingAction) {
        handleSleep(false);
    } else {
        tp_button.read();
        if (!handlingAction) { showPage(); }
    }
}

void handlePress() {
    time_out = millis();
    if (submenus[page] && submenus[page](false)) { return; }
    initialLoad = true;
    increasePage();
}

void increasePage() {
    page++;
    initialLoad = true;
    if (!pages[page] && !pages[page+1]) { page = 0; }
}

void showPage() {
    if (pages[page]) {
        max_time_out = timeOut[page] * 1000;
        if (max_time_out < 1000) { max_time_out = 10000; }
        pages[page](initialLoad);
        initialLoad = false;
    }
    showClock(page, pagesCount);
}

void handleAction() {
    handlingAction = true;
    if (actions[page]) {
        actions[page]();
        initialLoad = true;
    } else if (submenus[page]) {
        submenus[page](true);
    }
    handlingAction = false;
    time_out = millis();
}

uint8_t getTimeout() {
    uint32_t t =  100 - ((millis() - time_out) * 100 / max_time_out);
    if (t > 100) { return 100; }
    return t;
}

void home() { page = 0; initialLoad = true; showPage(); }
