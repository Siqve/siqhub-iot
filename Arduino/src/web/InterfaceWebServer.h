#ifndef CONTROLLERWEBSERVER_H
#define CONTROLLERWEBSERVER_H

#include "ESPAsyncWebServer.h"
#include "LED/LEDController.h"

class InterfaceWebServer {
public:
    explicit InterfaceWebServer(LEDController *ledController);
    void initServer();
private:
    LEDController ledController;
    AsyncWebServer server;
    void addRequestListeners();
    void onLandingPage();
    void onUpdate();
    void onMode();
};


#endif //CONTROLLERWEBSERVER_H
