#ifndef ARDUINO_STATICLEDMODE_H
#define ARDUINO_STATICLEDMODE_H


#include "LED/LEDMode.h"

class StaticLEDMode : public LEDMode {
public:
    StaticLEDMode(Adafruit_NeoPixel* LEDStrip, int pixelCount, std::function<void(int)> setFPS);
    void loop() override;
    void onUpdate(AsyncWebServerRequest *request) override;
};

#endif //ARDUINO_STATICLEDMODE_H
