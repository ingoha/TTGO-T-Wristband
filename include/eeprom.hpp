#ifndef Eeprom_H
#define Eeprom_H

#include <Arduino.h>
#include <EEPROM.h>

class Eeprom {
  private:
    const byte MAG_CALIBRATION_ADDRESS = 0x00;

  public:
    void storeMagBiasEEPROM(float *magbias);
    void getMagBiasEEPROM(float *magbias);
};

#endif