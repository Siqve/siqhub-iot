#include <HardwareSerial.h>
#include <ArduinoOTA.h>
#include "networking/WiFiSession.h"
#include "networking/web/WebServerManager.h"
#include "hardware/LED/LEDController.h"
#include <LittleFS.h>
#include "web/SupabaseClient.h"
#include "networking/web/SupabaseClient.h"

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

bool testing_fireTestRequest = false;

void setup() {
    systemSetup();
    webSetup();
    ledController.setup();

    Debug::DebugCommandHandler::getInstance().registerListener("test", [](std::string& command) {
        testing_fireTestRequest = true;
        IPAddress ip;
        Debug::Logger::soloDebugLog(("DNS: " + WiFi.dnsIP().toString()).c_str());

        if (WiFi.hostByName("google.com", ip)) {
            Serial.print("Resolved IP: ");
            Serial.println(ip);
        } else {
            Serial.println("DNS resolution failed");
        }
    });
}

void loop() {
    if (!wifiSession.assureConnection())
        return;
    if (testing_fireTestRequest) {
        testing_fireTestRequest = false;
//        supabaseClient.acquireToken();
    }

    ArduinoOTA.handle();
    supabaseClient.loop();
    ledController.loop();
}