#include "pages/page-bearing.hpp"

// HTTPServer myServer = HTTPServer();

uint32_t timeBearing = millis();

uint8_t requests = 0;

// void handleRoot(HTTPRequest * req, HTTPResponse * res);
// void handleEcho(HTTPRequest * req, HTTPResponse * res);

void pageBearing(bool initialLoading)
{
  if (initialLoading)
  {
    // deactivateWifi();
    initDrawBearing();
    initDrawTemperature();
    // initMPU();
    HAL::getInstance()->getMPU();
  }
  if (millis() - timeBearing > 300)
  {
    MPU* mpu = HAL::getInstance()->getMPU();
    refreshDrawBearing(mpu->getBearing());
    timeBearing = millis();
    float temperature = mpu->getTemperature();
    refreshDrawTemperature(temperature);
  }
}

void actionBearing() {
  /*
  msgInfo("Move wristband\nin 8 figure");
  int result = calibrateBearing();
  if (result < 0)
  {
    msgError("Calibration failed");
  }
  else
  {
    msgSuccess("Calibration Successful");
  }
  sleep(5);
  */
  /*
  activateWifi();
  ResourceNode * nodeRoot = new ResourceNode("/", "GET", &handleRoot);
  ResourceNode * nodeEchoPost = new ResourceNode("/", "POST", &handleEcho);
  myServer.registerNode(nodeRoot);
  myServer.registerNode(nodeEchoPost);
  myServer.start();
  uint32_t previousTime = millis();
  while (millis() - previousTime <= 10000) {
    myServer.loop();
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.drawString(String(requests), tft.width() / 2, 100);
  }
  */
}
