#ifndef CONTROLLERWEBSERVER_H
#define CONTROLLERWEBSERVER_H

#include "ESPAsyncWebServer.h"

class ControllerWebServer {
public:
    ControllerWebServer();
    void initServer();
private:
    AsyncWebServer server;
    int activeMode = 0;
    int staticColor = 0;

    void addRequestListeners();
    void onLandingPage();
    void onUpdate();
    void onMode();
};


#endif //CONTROLLERWEBSERVER_H
