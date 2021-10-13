#include "sleep.hpp"
#include "pins.hpp"
#include "hal.hpp"
#include "network.hpp"


void handleSleep(bool showMsg) {
    HAL* hal = HAL::getInstance();
    //tftSleep(showMsg);
    digitalWrite(LED_PIN, LOW);
    //mpuSleep();
    hal->getMPU()->mpuSleep();
    //tftSleep(false);
    hal->getTFT()->sleep(false);
    Network::getInstance()->getWIFI()->deactivateWifi();
    //rtcSleep();
    hal->getClock()->rtcSleep();
    //imuSleep();
    hal->getMPU()->imuSleep();
    pinMode(39, GPIO_MODE_INPUT);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 0);
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
}
