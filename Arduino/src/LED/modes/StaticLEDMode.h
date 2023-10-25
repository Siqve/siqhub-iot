#ifndef ARDUINO_STATICLEDMODE_H
#define ARDUINO_STATICLEDMODE_H


#include "LED/LEDMode.h"
#include "Constants.h"


class StaticLEDMode : public LEDMode {
public:
    StaticLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStrip, std::function<void(int)> setFPS);
    void onActivate() override;
    void loop() override;
    void onUpdate(AsyncWebServerRequest *request) override;
private:
    uint32_t staticColor = LEDConstants::DEFAULT_STATIC_COLOR;
};

#endif //ARDUINO_STATICLEDMODE_H
