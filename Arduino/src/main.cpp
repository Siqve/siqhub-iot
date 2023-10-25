#include <Arduino.h>
#include <ArduinoOTA.h>
#include "web/WiFiSession.h"
#include "web/WebServerManager.h"
#include "LED/LEDController.h"
#include <LittleFS.h>

LEDController ledController;
WiFiSession wifiSession;
WebServerManager webServer = WebServerManager(ledController);

void setup() {
    Serial.begin(115200);
    LittleFS.begin(); // Starts the LittleFS file system
    wifiSession.startSession();
    webServer.initServer();
    ledController.setup();
    ArduinoOTA.begin();
}

void loop() {
    if (!wifiSession.assureConnection())
        return;
    ArduinoOTA.handle();
    ledController.loop();
}