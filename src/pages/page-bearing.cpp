#include "pages/page-bearing.hpp"

const uint16_t PageBearing::timeout() { return 30;}

void PageBearing::draw(bool initialLoading)
{
  TFT* tft = hal->getTFT();
  if (initialLoading)
  {
    // deactivateWifi();
    tft->initDrawBearing();
    tft->initDrawTemperature();
  }
  if (millis() - timeBearing > 300)
  {
    MPU* mpu = hal->getMPU();
    mpu->update();
    tft->refreshDrawBearing(mpu->getBearing());
    timeBearing = millis();
    float temperature = mpu->getTemperature();
    tft->refreshDrawTemperature(temperature);
    char buf[100];
    MPU::xyz mag = mpu->getMag();
    sprintf(buf, "magX: %2f, magY: %2f, magZ: %2f", 
                    mag.x, mag.y, mag.z);
    Serial.println(buf);
  }
}

void PageBearing::action() {
  TFT* tft = hal->getTFT();
  tft->msgInfo("Move wristband\nin 8 figure");
  int result = hal->getMPU()->calibrateBearing();
  if (result < 0)
  {
    tft->msgError("Calibration failed");
  }
  else
  {
    tft->msgSuccess("Calibration Successful");
  }
  sleep(5);
}
