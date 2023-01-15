#include <sstream>
#include "InterfaceWebServer.h"
#include "web/generated/index.h"
#include "utilities/ColorUtils.h"


InterfaceWebServer::InterfaceWebServer() : server(AsyncWebServer(80)), activeMode(0) {

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
    server.on("/update", HTTP_GET, [this] (AsyncWebServerRequest *request) {
        String inputMessage;
        String inputParam;
        if (request->hasParam("mode")) {
            activeMode = request->getParam("mode")->value().toInt();
            Serial.print("Mode: ");
            Serial.println(activeMode);
        }
        if (request->hasParam("staticcolor")) {
            const char* hexStringParam = request->getParam("staticcolor")->value().c_str();
            Serial.println(ColorUtils::hexStringToColor(hexStringParam));
        }

        request->send(200, "text/plain", "OK");
    });
}

void InterfaceWebServer::onMode() {
    server.on("/mode", HTTP_GET, [this] (AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(activeMode).c_str());
    });
}
