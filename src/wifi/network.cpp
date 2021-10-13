#include "network.hpp"

Network* Network::instance = nullptr;

Network::Network() {
  ntp = new Ntp();
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
