#ifndef ARDUINO_STATICLEDMODE_H
#define ARDUINO_STATICLEDMODE_H


#include "LED/LEDMode.h"

class StaticLEDMode : public LEDMode {
public:
    StaticLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStrip, const std::function<void(int)>& setFPS);
    void onActivate() override;
    void loop() override;
    void onUpdate(AsyncWebServerRequest *request) override;
private:
    uint32_t defaultColor = 16711680;
};

#endif //ARDUINO_STATICLEDMODE_H
