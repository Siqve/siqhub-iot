#include <sstream>
#include <utility>
#include "InterfaceWebServer.h"
#include "web/generated/index.h"


InterfaceWebServer::InterfaceWebServer(std::shared_ptr<LEDController> ledControllerPtr) : server(AsyncWebServer(80)) {
    this->ledControllerPtr = std::move(ledControllerPtr);
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
    onDebug();
}

void InterfaceWebServer::onLandingPage() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });
}

void InterfaceWebServer::onUpdate() {
    server.on("/update", HTTP_GET, [this](AsyncWebServerRequest *request) {
        ledControllerPtr->incomingUpdate(request);
        request->send(200, "text/plain", "OK");
    });
}

//Request to get the active mode
void InterfaceWebServer::onMode() {
    server.on("/mode", HTTP_GET, [this](AsyncWebServerRequest *request) {
        int activeMode = ledControllerPtr->getActiveModeNumber();
        request->send(200, "text/plain", String(activeMode).c_str());
    });
}

void InterfaceWebServer::onDebug() {
    server.on("/debug", HTTP_GET, [this](AsyncWebServerRequest *request) {
        ledControllerPtr->incomingDebug(request);
        request->send(200, "text/plain", "OK");
    });
}
