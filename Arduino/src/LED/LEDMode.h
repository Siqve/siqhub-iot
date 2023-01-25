#ifndef ARDUINO_LEDMODE_H
#define ARDUINO_LEDMODE_H


#include <utility>

#include "Adafruit_NeoPixel.h"
#include "ESPAsyncWebServer.h"

class LEDMode {
public:
    LEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr) {
        this->LEDStripPtr = std::move(LEDStripPtr);
    }
    virtual ~LEDMode() = default;
    virtual void onActivate() {};
    virtual void loop() = 0;
    virtual void debugButtonClick() {};
    virtual void onUpdate(AsyncWebServerRequest *request) {};
    virtual String getSettings() {
        return {};
    };
    int getLEDStripPixelCount() {
        return LEDStripPtr->numPixels();
    }
protected:
//    int LEDStripPixelCount;
    std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr;
    std::function<void(int)> setFPS{};
};


#endif //ARDUINO_LEDMODE_H
