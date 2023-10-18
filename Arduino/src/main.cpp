#include <Arduino.h>
#include <ArduinoOTA.h>
#include "web/WiFiSession.h"
#include "web/InterfaceWebServer.h"
#include "Constants.h"
#include "LED/LEDController.h"
#include "NeoPixelBus.h"
#include "utilities/DebugManager.h"

std::shared_ptr<LEDController> ledController = std::make_shared<LEDController>();

WiFiSession wifiSession(WiFiConstants::WIFI_SSID, WiFiConstants::WIFI_PW);
DebugManager debugManager;

InterfaceWebServer webServer = InterfaceWebServer(ledController, debugManager);

//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(16);



void setup() {
    Serial.begin(115200);
    wifiSession.startSession();
    webServer.initServer();
    ledController->setup(LEDConstants::LED_DATA_PIN);
    ArduinoOTA.begin();
//    ledController->setup();
}

void loop() {
    wifiSession.assureConnection();
    ArduinoOTA.handle();
    wifiSession.assureConnection();

    ledController->loop();
    delay(1000);
}
