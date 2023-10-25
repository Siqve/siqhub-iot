#include <ESP8266WiFi.h>
#include "WiFiSession.h"
#include "Constants.h"
#include <iostream>

void WiFiSession::startSession() {
    WiFi.config(IPAddress(192, 168, 0, 200),
                IPAddress(255, 255, 255, 0),
                IPAddress(192, 168, 0, 1));
    WiFi.begin(wifi_ssid, wifi_pw);
}

bool lastWifiCheck_connected = false;
bool WiFiSession::assureConnection() {
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
void WiFiSession::printConnectingInfo() {
    unsigned long timeNow = millis();
    unsigned long timeLapsed = (timeNow - timeLastRun_ConnectWiFi) % ULONG_MAXVAL;
    if (timeLapsed < 2000)
        return;
    timeLastRun_ConnectWiFi = timeNow;
    logger.info(std::string("Connecting... (ssid: ") + wifi_ssid + ")");
}


void WiFiSession::printConnected() {
    logger.info("Successfully connected to WiFi!");
    logger.info(std::string("SSID: ") + wifi_ssid + ", IP-address: " + WiFi.localIP().toString().c_str());
}

