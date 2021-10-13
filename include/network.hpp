#ifndef NETWORK_H
#define NETWORK_H

#include "ntp.hpp"

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
  
  // getter
  public:
    Ntp* getNTP();
};

#endif