#include <HardwareSerial.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include "networking/WiFiManager.h"
#include "networking/web/WebServer.h"
#include "services/supabase/SupabaseRealtimeService.h"
#include "core/DeviceManager.h"
#include "debug/DebugCommandHandler.h"

WiFiManager wifiManager;
DeviceManager deviceManager;


void systemSetup() {
    Serial.begin(115200);
    LittleFS.begin();
    Debug::DebugCommandHandler::getInstance().initialize();
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
    SupabaseRealtimeService::getInstance().loop();
    deviceManager.loop();
}