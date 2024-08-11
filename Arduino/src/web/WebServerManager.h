#ifndef CONTROLLERWEBSERVER_H
#define CONTROLLERWEBSERVER_H

#include "ESPAsyncWebServer.h"
#include "LED/LEDController.h"
#include "debug/DebugCommandHandler.h"
#include "debug/Logger.h"

class WebServerManager {
public:
    explicit WebServerManager(LEDController& ledControllerPtr) :
            server(AsyncWebServer(80)), ledControllerPtr(ledControllerPtr),
            logger(Debug::Logger("WebServerManager")) {}

    void initServer();

private:
    AsyncWebServer server;
    LEDController& ledControllerPtr;
    Debug::Logger logger;

    void addRequestListeners();

    static void sendResponse(AsyncWebServerRequest* request, const std::string& responseJSON, int status);

    static void sendOKResponseJSON(AsyncWebServerRequest* request, const std::string& responseJSON = "");
    static void sendOKResponse(AsyncWebServerRequest* request, const std::string& responsePlain = "");
    static void sendBADResponsePlain(AsyncWebServerRequest* request, const std::string& responsePlain = "");

    void onLandingPage();
    void onDebug();
    void onUpdate();
    void onModeAndSettings();
    void onSettings();


    static const int RESPONSE_STATUS_HTTP_OK;
    static const int RESPONSE_STATUS_HTTP_BAD;
    static const char* RESPONSE_TYPE_PLAIN;
    static const char* RESPONSE_TYPE_HTML;
    static const char* RESPONSE_TYPE_JSON;
};


#endif //CONTROLLERWEBSERVER_H
