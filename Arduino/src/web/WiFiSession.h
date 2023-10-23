#ifndef WIFISESSON_H
#define WIFISESSON_H

class WiFiSession {
public:
    WiFiSession(const char* wifi_ssid, const char* wifi_pw);
    void startSession();
    bool assureConnection();
    void printWiFiIP();
private:
    const char* wifi_ssid, *wifi_pw;
    void connectWiFi();
};


#endif //WIFISESSON_H
