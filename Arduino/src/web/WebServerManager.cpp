#include <LittleFS.h>
#include "WebServerManager.h"
#include "utilities/TextUtils.h"

const int WebServerManager::RESPONSE_STATUS_HTTP_OK = 200;
const int WebServerManager::RESPONSE_STATUS_HTTP_BAD = 400;
const char* WebServerManager::RESPONSE_TYPE_JSON = "text/json";

void WebServerManager::initServer() {
    server.begin();
    logger.info("HTTP server started.");
    addRequestListeners();
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
        sendOKResponse(request);
    });
}

// Request to retrieve debug console
void WebServerManager::onDebug() {
    server.on("/debug", HTTP_GET, [this](AsyncWebServerRequest* request) {
        if (request->hasParam("get")) {
            String getParam = request->getParam("get")->value();
            if (getParam == "update-id") {
                sendOKResponse(request, std::to_string(Debug::Logger::getLogUpdateId()));
                return;
            }
        }
        else if (request->hasParam("cmd")) {
            String cmd = request->getParam("cmd")->value();
            DebugManager::getInstance().onDebugCommand(cmd.c_str());
            sendOKResponse(request);
            return;
        } else if (request->hasParam("boost")) {
            ledControllerPtr.incomingDebug();
            sendOKResponse(request);
            return;
        } else if (request->hasParam("console")) {
            sendOKResponse(request, TextUtils::replaceAll(Debug::Logger::getLogFeed(), "\n", "<br>"));
            return;
        }
        // TODO: Deprecate
        sendOKResponse(request, Debug::Logger::getLogFeed());
    });
}


// Request to receive an update
void WebServerManager::onUpdate() {
    server.on("/update", HTTP_GET, [this](AsyncWebServerRequest* request) {
        ledControllerPtr.incomingUpdate(request);
        sendOKResponse(request);
    });
}

// Request to retrieve active mode and settings
void WebServerManager::onModeAndSettings() {
    server.on("/mode", HTTP_GET, [this](AsyncWebServerRequest* request) {
        sendOKResponse(request, ledControllerPtr.getModeAndSettings().c_str());
    });
}

// Request to retrieve active mode
void WebServerManager::onSettings() {
    server.on("/settings", HTTP_GET, [this](AsyncWebServerRequest* request) {
        //Format: LED_PIXEL_COUNT,
        int LEDPixelCount = ledControllerPtr.getLEDStripPixelCount();
        std::string response = std::to_string(LEDPixelCount) + ",";
        sendOKResponse(request, response);
    });
}

void WebServerManager::sendOKResponse(AsyncWebServerRequest* request, const std::string& responsePlain) {
    sendResponse(request, "\"" + responsePlain + "\"", RESPONSE_STATUS_HTTP_OK);
}

void WebServerManager::sendOKResponseJSON(AsyncWebServerRequest* request, const std::string& responseJSON) {
    sendResponse(request, responseJSON, RESPONSE_STATUS_HTTP_OK);
}

void WebServerManager::sendBADResponsePlain(AsyncWebServerRequest* request, const std::string& responsePlain) {
    sendResponse(request, "\"" + responsePlain + "\"", RESPONSE_STATUS_HTTP_BAD);
}


void WebServerManager::sendResponse(AsyncWebServerRequest* request, const std::string& responseJSON, int status) {
    String finalResponse = String("{")
                           + "\"status\": " + status;
    if (!responseJSON.empty()) {
        finalResponse += ", \"response\": " + String(responseJSON.c_str());
    }
    finalResponse += "}";

    AsyncWebServerResponse* response = request->beginResponse(status, RESPONSE_TYPE_JSON, finalResponse);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    request->send(response);
}