#ifndef CONTROLLERWEBSERVER_H
#define CONTROLLERWEBSERVER_H

#include "ESPAsyncWebServer.h"
#include "LED/LEDController.h"
#include "utilities/DebugManager.h"

class WebServerManager {
public:
    explicit WebServerManager(LEDController& ledControllerPtr) :
            server(AsyncWebServer(80)), ledControllerPtr(ledControllerPtr),
            logger(DebugManager::getInstance().newLogger("WebServerManager")) {}

    void initServer();

private:
    AsyncWebServer server;
    LEDController& ledControllerPtr;
    DebugManager::Logger logger;

    void addRequestListeners();

    static void
    sendResponse(AsyncWebServerRequest* request, const char* responseText = "", int status = RESPONSE_STATUS_HTTP_OK,
                 const char* responseType = RESPONSE_TYPE_PLAIN);

    void onLandingPage();
    void onDebug();
    void onUpdate();
    void onModeAndSettings();
    void onSettings();


    static const int RESPONSE_STATUS_HTTP_OK;
    static const char* RESPONSE_TYPE_PLAIN;
    static const char* RESPONSE_TYPE_HTML;
};


#endif //CONTROLLERWEBSERVER_H
