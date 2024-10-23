#pragma once

#include "LedMode.h"
#include "ArduinoJson.h"

class FadeLedMode : public LedMode {
  public:
    FadeLedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip, std::function<void(int)> setFps);
    void loop() override;
    void onUpdate(const RequestWrapper& request) override;
    String getSettings() override;
    String getSettingsJSON();
private:
    void initialize(const JsonDocument& settings) override;
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