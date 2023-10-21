#include <sstream>
#include <utility>
#include "InterfaceWebServer.h"
#include "web/generated/index_html.h"
#include "web/generated/debug_html.h"

const int HTTP_OK = 200;
const String RESPONSE_TYPE_PLAIN = "text/plain";
const String RESPONSE_TYPE_HTML = "text/html";

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
    onLandingPage();
    onUpdate();
    onModeAndSettings();
    onSettings();
    onDebugPage();
    onDebugConsole();
}

void InterfaceWebServer::onLandingPage() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(HTTP_OK, RESPONSE_TYPE_HTML, index_html);
    });
}

void InterfaceWebServer::onUpdate() {
    server.on("/update", HTTP_GET, [this](AsyncWebServerRequest *request) {
        ledControllerPtr->incomingUpdate(request);
        request->send(HTTP_OK, RESPONSE_TYPE_PLAIN, "OK");
    });
}

// Request to get the active mode and settings
void InterfaceWebServer::onModeAndSettings() {
    server.on("/mode", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(HTTP_OK, RESPONSE_TYPE_PLAIN,
                      ledControllerPtr->getModeAndSettings().c_str());
    });
}

// Request to get the active mode
void InterfaceWebServer::onSettings() {
    server.on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request) {
        //Format: LED_PIXEL_COUNT,
        int LEDPixelCount = ledControllerPtr->getLEDStripPixelCount();
        String response = String(LEDPixelCount) + ",";
        request->send(HTTP_OK, RESPONSE_TYPE_PLAIN, response.c_str());
    });
}

// Request to load the debug page
void InterfaceWebServer::onDebugPage() {
    server.on("/debug", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("Incominggggggg");
        if (request->hasParam("cmd")) {
            String command = request->getParam("cmd")->value();
            debugManager.onDebug(command.c_str());
            request->send(HTTP_OK, RESPONSE_TYPE_PLAIN, "OK");
            return;
        }
        request->send_P(HTTP_OK, RESPONSE_TYPE_HTML, debug_html);
    });
}

// Request to retrieve debug console
void InterfaceWebServer::onDebugConsole() {
    server.on("/debug-console", HTTP_GET, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("get")) {
            String getParam = request->getParam("get")->value();
            if (getParam == "update-id") {
                request->send(HTTP_OK, RESPONSE_TYPE_PLAIN, std::to_string(debugManager.getLogUpdateId()).c_str());
                return;
            }
        }
        Serial.println("Sending feed.");
        request->send(HTTP_OK, RESPONSE_TYPE_PLAIN, debugManager.getLogFeed().c_str());
    });
}
