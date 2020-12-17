#include "calendar.hpp"

RTC_DATA_ATTR Appointment appointments[2][5];
Appointment empty = { "-", "-", 0, 0, 0, 0 };

void saveAppointment(uint8_t n, const char *name, const char *place,
                     uint8_t hfrom, uint8_t mfrom, uint8_t hto, uint8_t mto) {
  struct Appointment ap;
  strncpy(ap.name, name, 32);
  strncpy(ap.place, place, 32);
  ap.hfrom = hfrom;
  ap.mfrom = mfrom;
  ap.hto = hto;
  ap.mto = mto;
  ap.set = true;
  if (ap.hfrom >= 24) {
    ap.hfrom %= 24;
  }
  if (ap.hto >= 24) {
    ap.hto %= 24;
  }
  if (ap.hfrom > ap.hto) {
    ap.hto = ap.hfrom;
  }
  if ((ap.hfrom == ap.hto) && (ap.mfrom >= ap.mto) && (ap.hfrom != 0) &&
      (ap.mto != 0)) {
    ap.mto = ap.mfrom + 30;
  }
  if (ap.mfrom >= 60) {
    ap.hfrom++;
    ap.mfrom %= 60;
  }
  if (ap.mto >= 60) {
    ap.hto++;
    ap.mto %= 60;
  }
  if (ap.hfrom >= 24) {
    ap.hfrom %= 24;
  }
  if (ap.hto >= 24) {
    ap.hto %= 24;
  }
  appointments[0][n] = ap;
}

Appointment getAppointment(uint8_t n) {
  if (n >= 5) {
    return empty;
  }
  if (appointments[0][n].set) {
    return appointments[0][n];
  }
  return empty;
}
