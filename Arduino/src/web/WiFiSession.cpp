#include <ESP8266WiFi.h>
#include "WiFiSession.h"
#include "Constants.h"

WiFiSession::WiFiSession(const char* wifi_ssid, const char* wifi_pw)  {
    this->wifi_ssid = wifi_ssid;
    this->wifi_pw = wifi_pw;
}


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
            printWiFiIP();
            lastWifiCheck_connected = true;
        }
        return true;
    }
    if (lastWifiCheck_connected)
        lastWifiCheck_connected = false;
    connectWiFi();
    return false;
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
    Serial.printf("Successfully connected to %s...\r\n", wifi_ssid);
    Serial.printf("IP Address: %s\r\n", WiFi.localIP().toString().c_str());
}

