#pragma once

#include "debug/Logger.h"
#include "ESPAsyncWebServer.h"
#include "RequestWrapper.h"

class WebServer {
public:
    WebServer(const WebServer &) = delete;
    WebServer &operator=(const WebServer &) = delete;

    static WebServer &getInstance() {
        static WebServer instance;  // Guaranteed to be lazy initialized and destroyed correctly
        return instance;
    }

    void start();
    void registerPageCallback(const std::string &path,
                              const std::function<AsyncWebServerResponse *(const RequestWrapper &)> &callback);

private:
    WebServer() = default;

    AsyncWebServer server = AsyncWebServer(80);
    Debug::Logger logger = Debug::Logger("WebServerManager");

    void onLandingPage();
    void onDebugPage();
    void onLogger();
};