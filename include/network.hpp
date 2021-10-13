#ifndef NETWORK_H
#define NETWORK_H

#include "ntp.hpp"
#include "wristband-wifi.hpp"
class WIFI;
#include "wristband-ota.hpp"
class WifiOTA;
#include "mqtt.hpp"
class MQTT;

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
    MQTT* mqtt;
  
  // getter
  public:
    Ntp* getNTP();
    WIFI* getWIFI();
    WifiOTA* getWifiOTA();
    MQTT* getMQTT();
};

#endif