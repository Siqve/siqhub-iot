#pragma once

#include "debug/Logger.h"

class WiFiManager {
public:
    WiFiManager() : wifi_ssid(WIFI_SSID),
                    wifi_pw(WIFI_PW),
                    logger(Debug::Logger("WiFiSession")) {}

    void connect();
    bool assureConnection();
    void printConnected();
private:
    const char* wifi_ssid, * wifi_pw;
    void printConnectingInfo();
    Debug::Logger logger;

};