#include "battery.hpp"





void Battery::setupADC() {
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        vref = adc_chars.vref;
    }
}

Battery::Battery() {
    setupADC();
    pinMode(CHARGE_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    vref = 1100;
}

const float Battery::getVoltage() {
    uint16_t v = analogRead(BATT_ADC_PIN);
    float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    return battery_voltage;
}

const float Battery::getBusVoltage() {
    uint16_t v = analogRead(VBUS_PIN);
    float bus_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    return bus_voltage;
}

const uint8_t Battery::calcPercentage(float voltage) {
    // float percentage = (volts - BATTERY_MIN_V) * 100 / (BATTERY_MAX_V - BATTERY_MIN_V);
    if (voltage >= 4.2) { return 100; }
    if (voltage <= 3.0) { return 0; }
    for (uint8_t i = 0; batteryCurve[i].max > 0; i++) {
        if (voltage >= batteryCurve[i].max) {
            float max = batteryCurve[i].max; uint8_t per = batteryCurve[i].per;
            float vr = batteryCurve[i > 0 ? (i-1) : 0].max - max;
            float vp = (voltage - max) / vr;
            uint8_t pr = batteryCurve[i > 0 ? (i-1) : 0].per - per;
            float pp = per + pr * vp;
            if (pp > 100) { return 100; }
            if (pp < 0) { return 0; }
            return (uint8_t)pp;
        }
    }
    return 0;
}

// Set LED on if battery is charging
void Battery::updateBatteryChargeStatus() {
    digitalWrite(LED_PIN, isCharging());
}

const bool Battery::isCharging() {
    return !digitalRead(CHARGE_PIN);
}
