#include <HardwareSerial.h>
#include <ArduinoOTA.h>
#include "networking/WiFiSession.h"
#include "networking/web/WebServer.h"
#include "hardware/LED/LEDController.h"
#include <LittleFS.h>
#include "services/SupabaseService.h"

LEDController ledController;
WiFiSession wifiSession;
SupabaseService supabaseClient;


void systemSetup() {
    Serial.begin(115200);
    LittleFS.begin();
}

void webSetup() {
    wifiSession.startSession();
    WebServer::getInstance().init();
    ArduinoOTA.begin();
}

void setup() {
    systemSetup();
    webSetup();
    ledController.setup();
}

void loop() {
    if (!wifiSession.assureConnection())
        return;

    ArduinoOTA.handle();
    supabaseClient.loop();
    ledController.loop();
}