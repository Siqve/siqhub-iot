#ifndef CONTROLLERWEBSERVER_H
#define CONTROLLERWEBSERVER_H

#include "ESPAsyncWebServer.h"

class ControllerWebServer {
public:
    ControllerWebServer();
    void initServer();
private:
    AsyncWebServer server;
    int activeMode;

    void addRequestListeners();
    void onLandingPage();
    void onUpdate();
    void onMode();
};


#endif //CONTROLLERWEBSERVER_H
