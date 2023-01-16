#include <Arduino.h>

#include "web/WiFiSession.h"
#include "web/InterfaceWebServer.h"
#include "Constants.h"
#include "LED/LEDController.h"

WiFiSession wifiSession(WiFiConstants::WIFI_SSID, WiFiConstants::WIFI_PW);
LEDController ledController = LEDController();
InterfaceWebServer webServer = InterfaceWebServer(&ledController);


void setup() {
    Serial.begin(115200);
    wifiSession.startSession();
    webServer.initServer();
    ledController.setup();
}



void loop() {
    wifiSession.assureConnection();
    ledController.loop();
}
