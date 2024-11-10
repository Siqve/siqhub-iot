#pragma once

#include "LedMode.h"
#include "ArduinoJson.h"

class FadeLedMode : public LedMode {
  public:
    FadeLedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip, std::function<void(int)> setFps);
    void loop() override;
    void handleUpdate(const JsonDocument& settings) override;
private:
    void cycleFade();
    void updateFps();
    void incrementHue();

    int ledFps;
    int ledSpeed;
    int ledPixelHueStep;
    int ledBrightness;

    uint16_t currentHue = 0;
    bool reverse = false;
};