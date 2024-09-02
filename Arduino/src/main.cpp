#include <HardwareSerial.h>
#include <ArduinoOTA.h>
#include "web/WiFiSession.h"
#include "web/WebServerManager.h"
#include "LED/LEDController.h"
#include <LittleFS.h>
#include "web/SupabaseClient.h"

LEDController ledController;
WiFiSession wifiSession;
SupabaseClient supabaseClient;


void systemSetup() {
    Serial.begin(115200);
    LittleFS.begin();
}

void webSetup() {
    wifiSession.startSession();
    WebServerManager::getInstance().init();
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