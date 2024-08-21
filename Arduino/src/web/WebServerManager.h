#ifndef WEBSERVERMANAGER_H
#define WEBSERVERMANAGER_H

#include "debug/Logger.h"
#include "ESPAsyncWebServer.h"
#include "RequestWrapper.h"

class WebServerManager {
public:
    WebServerManager(const WebServerManager&) = delete;
    WebServerManager& operator=(const WebServerManager&) = delete;

    static WebServerManager& getInstance() {
        static WebServerManager instance;  // Guaranteed to be lazy initialized and destroyed correctly
        return instance;
    }

    void init();
    void registerPageCallback(const std::string& path,
                              const std::function<AsyncWebServerResponse*(const RequestWrapper&)>& callback);

private:
    WebServerManager() {}

    AsyncWebServer server = AsyncWebServer(80);
    Debug::Logger logger = Debug::Logger("WebServerManager");

    void onLandingPage();
    void onLogger();
};


#endif //WEBSERVERMANAGER_H
