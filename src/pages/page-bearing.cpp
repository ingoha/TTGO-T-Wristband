#include "pages/page-bearing.hpp"

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
    tft->refreshDrawBearing(mpu->getBearing());
    timeBearing = millis();
    float temperature = mpu->getTemperature();
    tft->refreshDrawTemperature(temperature);
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
