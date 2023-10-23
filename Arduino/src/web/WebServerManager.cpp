#include <sstream>
#include <utility>
#include <LittleFS.h>
#include "WebServerManager.h"

const int WebServerManager::RESPONSE_STATUS_HTTP_OK = 200;
const char* WebServerManager::RESPONSE_TYPE_PLAIN = "text/plain";
const char* WebServerManager::RESPONSE_TYPE_HTML = "text/html";

WebServerManager::WebServerManager(std::shared_ptr<LEDController> ledControllerPtr, DebugManager& debugManager) :
        server(AsyncWebServer(80)), debugManager(debugManager) {
    this->ledControllerPtr = std::move(ledControllerPtr);
}

void WebServerManager::initServer() {
    server.begin();
    addRequestListeners();
    debugManager.logger().info("HTTP server started");
}


void WebServerManager::addRequestListeners() {
    onLandingPage();
    onDebug();
    onUpdate();
    onModeAndSettings();
    onSettings();
}

// Just used to indicate that server is running
void WebServerManager::onLandingPage() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        sendResponse(request, "Ok");
    });
}

// Request to retrieve debug console
void WebServerManager::onDebug() {
    server.on("/debug", HTTP_GET, [this](AsyncWebServerRequest* request) {
        if (request->hasParam("get")) {
            String getParam = request->getParam("get")->value();
            if (getParam == "update-id") {
                sendResponse(request, std::to_string(debugManager.logger().getLogUpdateId()).c_str());
                return;
            }
        }
        Serial.println("Sending feed.");
        sendResponse(request, debugManager.logger().getLogFeed().c_str());
    });
}


// Request to receive an update
void WebServerManager::onUpdate() {
    server.on("/update", HTTP_GET, [this](AsyncWebServerRequest* request) {
        ledControllerPtr->incomingUpdate(request);
        sendResponse(request);
    });
}

// Request to retrieve active mode and settings
void WebServerManager::onModeAndSettings() {
    server.on("/mode", HTTP_GET, [this](AsyncWebServerRequest* request) {
        sendResponse(request, ledControllerPtr->getModeAndSettings().c_str());
    });
}

// Request to retrieve active mode
void WebServerManager::onSettings() {
    server.on("/settings", HTTP_GET, [this](AsyncWebServerRequest* request) {
        //Format: LED_PIXEL_COUNT,
        int LEDPixelCount = ledControllerPtr->getLEDStripPixelCount();
        String response = String(LEDPixelCount) + ",";
        sendResponse(request, response.c_str());
    });
}


void WebServerManager::sendResponse(AsyncWebServerRequest* request, const char* responseText, int status,
                                    const char* responseType) {
    AsyncWebServerResponse* response = request->beginResponse(status, responseType, responseText);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    request->send(response);
}
