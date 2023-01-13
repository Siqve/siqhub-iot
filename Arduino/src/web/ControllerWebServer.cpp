#include "ControllerWebServer.h"
#include "index.h"


ControllerWebServer::ControllerWebServer() : server(AsyncWebServer(80)), activeMode(0) {

}

void ControllerWebServer::initServer() {
    addRequestListeners();

    server.begin();
    Serial.println("HTTP server started");
}


void ControllerWebServer::addRequestListeners() {
    onLandingPage();
    onUpdate();
    onMode();
}

void ControllerWebServer::onLandingPage() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });
}

void ControllerWebServer::onUpdate() {
    server.on("/update", HTTP_GET, [this] (AsyncWebServerRequest *request) {
        String inputMessage;
        String inputParam;
        if (request->hasParam("mode")) {
            activeMode = request->getParam("mode")->value().toInt();
            Serial.print("Mode: ");
            Serial.println(activeMode);
        }
        request->send(200, "text/plain", "OK");
    });
}

void ControllerWebServer::onMode() {
    server.on("/mode", HTTP_GET, [this] (AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(activeMode).c_str());
    });
}
