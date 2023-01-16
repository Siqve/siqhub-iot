#ifndef ARDUINO_LEDMODE_H
#define ARDUINO_LEDMODE_H


#include "Adafruit_NeoPixel.h"
#include "ESPAsyncWebServer.h"

class LEDMode {
public:
    virtual ~LEDMode() = default;
    virtual void loop() = 0;
    virtual void testButtonClick() {};
    virtual void onUpdate(AsyncWebServerRequest *request) {};
protected:
    int LEDStripPixelCount;
    Adafruit_NeoPixel LEDStrip;
    std::function<void(int)> setFPS{};
};

#endif //ARDUINO_LEDMODE_H
