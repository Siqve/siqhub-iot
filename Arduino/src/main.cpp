#include <Arduino.h>
#include <ArduinoOTA.h>
#include "web/WiFiSession.h"
#include "web/WebServerManager.h"
#include "Constants.h"
#include "LED/LEDController.h"
#include "utilities/DebugManager.h"
#include <LittleFS.h>

std::shared_ptr<LEDController> ledController = std::make_shared<LEDController>();
DebugManager debugManager;

WiFiSession wifiSession(WiFiConstants::WIFI_SSID, WiFiConstants::WIFI_PW);
WebServerManager webServer  = WebServerManager(ledController, debugManager);



void setup() {
    Serial.begin(115200);
    LittleFS.begin();

    wifiSession.startSession();
    webServer.initServer();
    ledController->setup(LEDConstants::LED_DATA_PIN);
    ArduinoOTA.begin();
//    ledController->setup();
}

void loop() {
    if (!wifiSession.assureConnection())
        return;
    ArduinoOTA.handle();
    ledController->loop();
}
