#include <Arduino.h>
#include <EasyButton.h>
#include "pages.hpp"
#include "pins.hpp"
#include "hal.hpp"
#include "network.hpp"
#include "pages/page-home.hpp"
#include "pages/page-battery.hpp"
#include "pages/page-ota.hpp"
#include "pages/page-gyro.hpp"
#include "pages/page-bearing.hpp"
#include "pages/page-mqtt.hpp"
#include "sleep.hpp"

Pages::Pages() {
    hal = HAL::getInstance();
    network = Network::getInstance();
    Serial.println("Setup touch button...");
    tp_button = new EasyButton(TP_PIN_PIN, 80, true, false);
    initButton();
    
    pageList = new std::vector<AbstractPage*>();
    // FIXME really here?
    pageList->push_back(new PageHome());
    pageList->push_back(new PageBattery());
    pageList->push_back(new PageGyro());
    pageList->push_back(new PageBearing());
    pageList->push_back(new PageOTA());
    pageList->push_back(new PageMqtt());

    page = 0;
    showPage();
    //for (pagesCount = 0; (pages[pagesCount] || pages[pagesCount+1]) && pagesCount < 10; pagesCount++) {}
}

/*
typedef void(*Page)(bool);
Page pages[] = {
    pageCalendar,
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
*/

/*
typedef void(*Action)();
Action actions[] = {
    actionCalendar, // NULL,
    actionClock,
    actionCounter,
    NULL,
    actionBearing,
    NULL,
    waitOta,
    NULL,
    NULL
};
*/

/*
bool submenu(int8_t press);
struct Submenu {
    const char **names;
    //Action *actions;
};
*/

const char *menuOptions[] = {
    "Play",
    "Next",
    "Prev",
    "WSTECZ",
    NULL
};


void sendPlay() { 
    Network::getInstance()->getMQTT()->MQTTpublish("musiccmd", "play-pause"); 
}
void sendNext() { 
    Network::getInstance()->getMQTT()->MQTTpublish("musiccmd", "next");
}
void sendPrev() { 
    Network::getInstance()->getMQTT()->MQTTpublish("musiccmd", "previous");
}
void sendStop() { 
    Network::getInstance()->getMQTT()->MQTTpublish("musiccmd", "stop");
}

/*
Action menuActions[] = {
    sendPlay,
    sendNext,
    sendPrev,
    NULL,
    NULL,
};

Submenu mainMenu = { menuOptions, menuActions };

Submenu *submenus[] = {
    NULL,
    NULL,
    NULL,
    &mainMenu,
    NULL,
    // submenuTemperature,
    NULL,
    NULL,
    NULL,
    NULL
};
*/

void Pages::initButton() {
  pinMode(TP_PWR_PIN, PULLUP);
  digitalWrite(TP_PWR_PIN, HIGH);
  tp_button->begin();
  // cf. https://github.com/evert-arias/EasyButton/issues/56
  tp_button->onPressedFor(1000, [this]() {
		this->handleAction();
	});
  tp_button->onPressed([this]() {
    this->handlePress();
	});
}

void Pages::refreshTimer() {
    time_out = millis();
}

void Pages::handleUi() {
    if (!hal->getBattery()->isCharging()) { 
        digitalWrite(LED_PIN, digitalRead(TP_PIN_PIN)); 
    }
    tp_button->read();
    if (hal->getBattery()->getBusVoltage() > 4.0) {
        if (!handlingAction) { showPage(); }
        return;
    }
    if (millis() - time_out > max_time_out && !handlingAction) {
        handleSleep(false);
    } else {
        tp_button->read();
        if (!handlingAction) { showPage(); }
    }
}

void Pages::handlePress() {
    time_out = millis();
    //if (submenus[page] && submenu(1)) { return; }
    initialLoad = true;
    increasePage();
}

void Pages::increasePage() {
    page++;
    initialLoad = true;
    // FIXME auto sleep when increasing from last page???
    if(page >= pageList->size()) { page = 0; }
    //if (!pages[page] && !pages[page+1]) { page = 0; }
}

void Pages::showPage() {
    TFT* tft = hal->getTFT();
    bool cleared = false;
    /*
    if (submenus[page] && submenu(0)) {
            if (cleared) { submenu(-2); }
            tft->drawBottomBar(getTimeout(), TFT_BLUE);
            return;
    }
    */
    if (pageList->at(page)) {
        max_time_out = timeOut[page] * 1000;
        if (max_time_out < 1000) { max_time_out = 10000; }
        pageList->at(page)->draw(cleared || initialLoad);
        initialLoad = false;
    }
}

void Pages::handleAction() {
    handlingAction = true;
    if(pageList->at(page)) {
        pageList->at(page)->action();
        initialLoad = true;
    }
    /*
    if (actions[page]) {
        actions[page]();
        initialLoad = true;
    } else if (submenus[page]) {
        submenu(2);
    }
    */
    handlingAction = false;
    time_out = millis();
}

const uint8_t Pages::getTimeout() {
    uint32_t t =  100 - ((millis() - time_out) * 100 / max_time_out);
    if (t > 100) { return 100; }
    return t;
}

void Pages::home() { page = 0; initialLoad = true; showPage(); }

/*
#define DmenuOptions submenus[page]->names
#define DmenuActions submenus[page]->actions

bool submenu(int8_t press) {
    TFT* tft = HAL::getInstance()->getTFT();
    if (press == -2) {
        tft->drawMenuPointer(menu, DmenuOptions[menu] != NULL);
        tft->drawOptions(DmenuOptions);
        return menu >= 0;
    }
    if (press < 0) { menu = -1; initialLoad = true; return false; }
    if (press == 0 && menu < 0) { return false; }
    if (!DmenuOptions[0]) { return false; }
    if (press == 2 && menu < 0) {
        tft->drawOptions(DmenuOptions);
        menu = 0;
        tft->drawMenuPointer(menu, DmenuOptions[menu] != NULL);
        return true;
    }
    if (menu < 0) { return false; }
    if (press == 2 && DmenuActions[menu]) {
        DmenuActions[menu]();
        return true;
    }
    if (press == 2 && !DmenuActions[menu]) {
        tft->clearScreen();
        menu = -1;
        initialLoad = true;
        return false;
    }
    if (press == 1) {
        menu++;
        if (DmenuOptions[menu]) {
            tft->drawMenuPointer(menu, DmenuOptions[menu] != NULL);
        }
    }
    if (!DmenuOptions[menu]) {
        menu = 0;
        tft->drawMenuPointer(menu, DmenuOptions[menu] != NULL);
    }
    // drawMenuPointer(menu, OPTIONS_TEMPERATURE);
    return true;
}
*/
