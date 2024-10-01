#include <HardwareSerial.h>
#include <ArduinoOTA.h>
#include "networking/WiFiManager.h"
#include "networking/web/WebServer.h"
#include "hardware/LED/LEDController.h"
#include "services/SupabaseService.h"
#include "core/DeviceManager.h"

LEDController ledController;
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
    ledController.setup();
}

void loop() {
    if (!wifiManager.assureConnection())
        return;

    ArduinoOTA.handle();
    SupabaseService::getInstance().loop();
    ledController.loop();
    deviceManager.loop();
}