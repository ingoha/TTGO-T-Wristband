#include <Arduino.h>
#pragma once

struct Appointment {
    char name[32];
    char place[32];
    uint8_t hfrom;
    uint8_t mfrom;
    uint8_t hto;
    uint8_t mto;
    bool set;
};

void saveAppointment(uint8_t n, const char *name, const char *place,
                     uint8_t hfrom, uint8_t mfrom, uint8_t hto, uint8_t mto);

Appointment getAppointment(uint8_t n);
