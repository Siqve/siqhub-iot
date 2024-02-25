#ifndef WIFISESSON_H
#define WIFISESSON_H

#include "utilities/DebugManager.h"
#include "Constants.h"

class WiFiSession {
public:
    WiFiSession() : wifi_ssid(WIFI_SSID),
                    wifi_pw(WIFI_PW),
                    logger(DebugManager::getInstance().newLogger(
                            "WiFiSession")) {}

    void startSession();
    bool assureConnection();
    void printConnected();
private:
    const char* wifi_ssid, * wifi_pw;
    void printConnectingInfo();
    DebugManager::Logger logger;

};


#endif //WIFISESSON_H
