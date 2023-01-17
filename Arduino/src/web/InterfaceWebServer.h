#ifndef CONTROLLERWEBSERVER_H
#define CONTROLLERWEBSERVER_H

#include "ESPAsyncWebServer.h"
#include "LED/LEDController.h"

class InterfaceWebServer {
public:
    explicit InterfaceWebServer(std::shared_ptr<LEDController> ledController);
    void initServer();
private:
    std::shared_ptr<LEDController> ledControllerPtr;
    AsyncWebServer server;
    void addRequestListeners();
    void onLandingPage();
    void onUpdate();
    void onMode();
    void onDebug();
};


#endif //CONTROLLERWEBSERVER_H
