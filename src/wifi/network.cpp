#include "network.hpp"

Network* Network::instance = nullptr;

Network::Network() {
  ntp = new Ntp();
  wifi = new WIFI();
  ota = new WifiOTA();
}

Network* Network::getInstance() {
  if(Network::instance == nullptr) {
    Network::instance = new Network();
  }
  return Network::instance;
}

Ntp* Network::getNTP() {
  return ntp;
}

WIFI* Network::getWIFI() {
  return wifi;
}

WifiOTA* Network::getWifiOTA() {
  return ota;
}