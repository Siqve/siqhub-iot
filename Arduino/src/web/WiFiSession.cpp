#include <ESP8266WiFi.h>
#include "WiFiSession.h"
#include "constants.h"

WiFiSession::WiFiSession(const char* wifi_ssid, const char* wifi_pw)  {
    this->wifi_ssid = wifi_ssid;
    this->wifi_pw = wifi_pw;
}


void WiFiSession::startSession() {
    WiFi.begin(wifi_ssid, wifi_pw);
}

bool lastWifiCheck_connected = false;
void WiFiSession::assureConnection() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!lastWifiCheck_connected) {
            printWiFiIP();
            lastWifiCheck_connected = true;
        }
        return;
    }
    if (lastWifiCheck_connected)
        lastWifiCheck_connected = false;
    connectWiFi();
}

unsigned long timeLastRun_ConnectWiFi;
void WiFiSession::connectWiFi() {
    unsigned long timeNow = millis();
    unsigned long timeLapsed = (timeNow - timeLastRun_ConnectWiFi) % ULONG_MAXVAL;
    if (timeLapsed < 1000)
        return;
    timeLastRun_ConnectWiFi = timeNow;
    Serial.printf("Connecting to WiFI. SSID: %s...\r\n", wifi_ssid);
    Serial.println();
}


void WiFiSession::printWiFiIP() {
    Serial.printf("Successfully connected to to %s...\r\n", wifi_ssid);
    Serial.printf("IP Address: %s\r\n", WiFi.localIP().toString().c_str());
}

