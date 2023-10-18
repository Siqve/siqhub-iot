#ifndef CONTROLLERWEBSERVER_H
#define CONTROLLERWEBSERVER_H

#include "ESPAsyncWebServer.h"
#include "LED/LEDController.h"
#include "utilities/DebugManager.h"

class InterfaceWebServer {
public:
    explicit InterfaceWebServer(std::shared_ptr<LEDController> ledController, DebugManager& debugManager);
    void initServer();

private:
    std::shared_ptr<LEDController> ledControllerPtr;
    AsyncWebServer server;
    DebugManager &debugManager;

    void addRequestListeners();
    void onLandingPage();
    void onUpdate();
    void onModeAndSettings();
    void onSettings();
    void onDebugPage();
    void onDebugConsole();
};


#endif //CONTROLLERWEBSERVER_H
