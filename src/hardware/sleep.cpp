#include "sleep.hpp"

void handleSleep(bool showMsg) {
    //tftSleep(showMsg);
    digitalWrite(LED_PIN, LOW);
    //mpuSleep();
    HAL::getInstance()->getMPU()->mpuSleep();
    //tftSleep(false);
    HAL::getInstance()->getTFT()->sleep(false);
    deactivateWifi();
    //rtcSleep();
    HAL::getInstance()->getClock()->rtcSleep();
    //imuSleep();
    HAL::getInstance()->getMPU()->imuSleep();
    pinMode(39, GPIO_MODE_INPUT);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 0);
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_disable_rom_logging();
    esp_deep_sleep_start();
}
