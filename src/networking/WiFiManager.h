#pragma once

#include "debug/Logger.h"

class WiFiManager {
public:
    static void connect();
    bool assureConnection();
    void printConnected();
private:
    void printConnectingInfo();
    Debug::Logger logger = Debug::Logger("WiFiManager");

};