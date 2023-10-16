#include <sstream>
#include <utility>
#include "InterfaceWebServer.h"
#include "web/generated/index_html.h"
#include "web/generated/debug_html.h"


InterfaceWebServer::InterfaceWebServer(std::shared_ptr<LEDController> ledControllerPtr, DebugManager &debugManager) :
        server(AsyncWebServer(80)), debugManager(debugManager) {
    this->ledControllerPtr = std::move(ledControllerPtr);
}

void InterfaceWebServer::initServer() {
    addRequestListeners();
    server.begin();
    Serial.println("HTTP server started");
}


void InterfaceWebServer::addRequestListeners() {
//    onLandingPage();
//    onUpdate();
//    onModeAndSettings();
//    onSettings();
    onDebug();
    onDebugConsole();
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

//Request to get the active mode and settings
void InterfaceWebServer::onModeAndSettings() {
    server.on("/mode", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "text/plain",
                      ledControllerPtr->getModeAndSettings().c_str());
    });
}

//Request to get the active mode
void InterfaceWebServer::onSettings() {
    server.on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request) {
        //Format: LED_PIXEL_COUNT,
        int LEDPixelCount = ledControllerPtr->getActiveMode()->getLEDStripPixelCount();
        String response = String(LEDPixelCount) + ",";
        request->send(200, "text/plain", response.c_str());
    });
}

void InterfaceWebServer::onDebug() {
    server.on("/debug", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Incominggggggg");
        if (request->hasParam("cmd")) {
            String command = request->getParam("cmd")->value();
            debugManager.onDebug(command.c_str());
            request->send_P(200, "text/html", "OK");
            return;
        }
        request->send_P(200, "text/html", debug_html);
    });
}


void InterfaceWebServer::onDebugConsole() {
    server.on("/debug-console", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("get")) {
            String getParam = request->getParam("get")->value();
            if (getParam == "update-id") {
                request->send_P(200, "text/html", std::to_string(debugManager.getLogUpdateId()).c_str());
                return;
            }
        }
        Serial.println("Sending feed.");
        request->send_P(200, "text/html", debugManager.getLogFeed().c_str());
    });
}
