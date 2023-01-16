#include <sstream>
#include "InterfaceWebServer.h"
#include "web/generated/index.h"


InterfaceWebServer::InterfaceWebServer(LEDController* ledController) : server(AsyncWebServer(80)) {
    this->ledController = *ledController;
}

void InterfaceWebServer::initServer() {
    addRequestListeners();

    server.begin();
    Serial.println("HTTP server started");
}


void InterfaceWebServer::addRequestListeners() {
    onLandingPage();
    onUpdate();
    onMode();
}

void InterfaceWebServer::onLandingPage() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });
}

void InterfaceWebServer::onUpdate() {
    server.on("/update", HTTP_GET, [this](AsyncWebServerRequest *request) {
        ledController.incomingUpdate(request);
        request->send(200, "text/plain", "OK");
    });
}

//Request to get the active mode
void InterfaceWebServer::onMode() {
    server.on("/mode", HTTP_GET, [this](AsyncWebServerRequest *request) {
        int activeMode = ledController.getActiveMode();
        request->send(200, "text/plain", String(activeMode).c_str());
    });
}
