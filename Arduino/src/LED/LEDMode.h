#ifndef ARDUINO_LEDMODE_H
#define ARDUINO_LEDMODE_H

#include <utility>
#include <memory>
#include "Adafruit_NeoPixel.h"
#include "ESPAsyncWebServer.h"
#include "NeoPixelBus.h"

class LEDMode {
public:
    virtual ~LEDMode() = default;

    virtual void loop() = 0;

    virtual void onActivate() {};
    virtual void debugButtonClick() {};

    virtual void onUpdate(AsyncWebServerRequest* request) {};

    virtual String getSettings() {
        return {};
    };
protected:
    explicit LEDMode(std::shared_ptr<NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>>& LEDStripPtr,
                     std::function<void(int)> setFPS) : LEDStripPtr(LEDStripPtr) {
        this->setFPS = std::move(setFPS);
    }

    std::shared_ptr<NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>>& LEDStripPtr;
    std::function<void(int)> setFPS{};
};


#endif //ARDUINO_LEDMODE_H
