#ifndef ARDUINO_MUSICLEDMODE_H
#define ARDUINO_MUSICLEDMODE_H


#include "LED/LEDMode.h"

class MusicLEDMode : public LEDMode {
public:
    MusicLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, std::function<void(int)> setFPS);
    void onActivate() override;
    void loop() override;
    void onUpdate(AsyncWebServerRequest *request) override;
    void debugButtonClick() override;
    String getSettings() override;
private:
    void cycleFade();
    void beat();
    void incrementHue();
    float speedBoost = 0;
    int baseFPS;
    int speed;
    int pixelColorHop;
    int bounce;
    void updateFPS();
    float getSpeedBoost();
    float stabilizeSpeed();
    bool reverse = false;

};

#endif //ARDUINO_MUSICLEDMODE_H
