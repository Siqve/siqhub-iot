#include "WiFiManager.h"
#include "Constants.h"

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
    WiFi.begin(wifi_ssid, wifi_pw);
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

unsigned long timeLastRun_ConnectWiFi;
void WiFiManager::printConnectingInfo() {
    unsigned long timeNow = millis();
    unsigned long timeLapsed = (timeNow - timeLastRun_ConnectWiFi) % ULONG_MAXVAL;
    if (timeLapsed < 2000)
        return;
    timeLastRun_ConnectWiFi = timeNow;
    logger.info(std::string("Connecting... (ssid: ") + wifi_ssid + ")");
}


void WiFiManager::printConnected() {
    logger.info("Successfully connected to WiFi!");
    logger.info(std::string("SSID: ") + wifi_ssid + ", IP-address: " + WiFi.localIP().toString().c_str());
}

