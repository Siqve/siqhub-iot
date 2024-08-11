#ifndef WIFISESSON_H
#define WIFISESSON_H

#include "debug/DebugCommandHandler.h"
#include "Constants.h"
#include "debug/Logger.h"

class WiFiSession {
public:
    WiFiSession() : wifi_ssid(WIFI_SSID),
                    wifi_pw(WIFI_PW),
                    logger(Debug::Logger("WiFiSession")) {}

    void startSession();
    bool assureConnection();
    void printConnected();
private:
    const char* wifi_ssid, * wifi_pw;
    void printConnectingInfo();
    Debug::Logger logger;

};


#endif //WIFISESSON_H
