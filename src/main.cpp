#include <HardwareSerial.h>
#include <ArduinoOTA.h>
#include "networking/WiFiManager.h"
#include "networking/web/WebServer.h"
#include "services/supabase/SupabaseService.h"
#include "core/DeviceManager.h"

WiFiManager wifiManager;
DeviceManager deviceManager;


void systemSetup() {
    Serial.begin(115200);
}

void networkingSetup() {
    WiFiManager::connect();
    WebServer::getInstance().start();
    ArduinoOTA.begin();
}

void setup() {
    systemSetup();
    networkingSetup();
}

void loop() {
    if (!wifiManager.assureConnection())
        return;

    ArduinoOTA.handle();
    SupabaseService::getInstance().loop();
    deviceManager.loop();
}