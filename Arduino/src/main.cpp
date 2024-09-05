#include <HardwareSerial.h>
#include <ArduinoOTA.h>
#include "networking/WiFiManager.h"
#include "networking/web/WebServer.h"
#include "hardware/LED/LEDController.h"
#include "services/SupabaseService.h"

LEDController ledController;
WiFiManager wifiSession;
SupabaseService supabaseClient;


void systemSetup() {
    Serial.begin(115200);
}

void webSetup() {
    wifiSession.connect();
    WebServer::getInstance().start();
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