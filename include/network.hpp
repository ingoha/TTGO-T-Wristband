#ifndef NETWORK_H
#define NETWORK_H

#include "ntp.hpp"
#include "wristband-wifi.hpp"
class WIFI;
#include "wristband-ota.hpp"
class WifiOTA;

class Network {
  // singleton
  private:
    static Network* instance;
    Network();

  public:
    static Network* getInstance();

  // components
  private:
    Ntp* ntp;
    WIFI* wifi;
    WifiOTA* ota;
  
  // getter
  public:
    Ntp* getNTP();
    WIFI* getWIFI();
    WifiOTA* getWifiOTA();
};

#endif