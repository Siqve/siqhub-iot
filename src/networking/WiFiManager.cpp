#include "WiFiManager.h"
#include "utils/TimeUtils.h"

#ifdef ESP32
  #include <WiFi.h>  // Include the WiFi library for ESP32
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>  // Include the WiFi library for ESP8266
#else
  #error "Unsupported board! Please select an ESP32 or ESP8266 board."
#endif

void WiFiManager::connect() {
    WiFi.config(IPAddress(192, 168, 0, DEVICE_IP),
                IPAddress(192, 168, 0, 1),
                IPAddress(255, 255, 255, 0),
                IPAddress(1, 1, 1, 1));
    WiFi.begin(WIFI_SSID, WIFI_PW);
}

bool lastWifiCheck_connected = false;
bool WiFiManager::assureConnection() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!lastWifiCheck_connected) {
            printConnected();
            lastWifiCheck_connected = true;
        }
        return true;
    }
    if (lastWifiCheck_connected) {
        logger.warn("Lost internet connection.");
        lastWifiCheck_connected = false;
    }
    printConnectingInfo();
    return false;
}

static uint32_t lastPrintTimeMillis;
void WiFiManager::printConnectingInfo() {
    if (!TimeUtils::isMillisElapsed(millis(), lastPrintTimeMillis, 3000)) {
        return;
    }
    lastPrintTimeMillis = millis();
    logger.info(std::string("Connecting... (SSID: ") + WIFI_SSID + ")");
}


void WiFiManager::printConnected() {
    logger.info("Successfully connected to WiFi!");
    logger.info(std::string("SSID: ") + WIFI_SSID + ", IP-address: " + WiFi.localIP().toString().c_str());
}

