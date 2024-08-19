#include <Arduino.h>
#include <ArduinoOTA.h>
#include "web/WiFiSession.h"
#include "web/WebServerManager.h"
#include "LED/LEDController.h"
#include <LittleFS.h>

LEDController ledController;
WiFiSession wifiSession;
WebServerManager webServer = WebServerManager(ledController);


void systemSetup() {
    Serial.begin(115200);
    LittleFS.begin();
}

void webSetup() {
    wifiSession.startSession();
    webServer.initServer();
    ArduinoOTA.begin();
}

void setup() {
    systemSetup();
    webSetup();

    supabaseClient.start();
    ledController.setup();
}


void loop() {
    if (!wifiSession.assureConnection())
        return;
    ArduinoOTA.handle();
    ledController.loop();
}