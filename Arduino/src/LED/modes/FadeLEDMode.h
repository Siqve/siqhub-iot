#pragma once

#include "../LEDMode.h"

class FadeLEDMode : public LEDMode {
  public:
    FadeLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, std::function<void(int)> setFPS);
    void loop() override;
    void onUpdate(const RequestWrapper& request) override;
    String getSettings() override;
    String getSettingsJSON();
private:
    void onActivate() override;
    void cycleFade();
    void updateFps();
    void incrementHue();

    int ledFPS;
    int ledSpeed;
    int ledPixelHueStep;
    int ledBrightness;

    uint16_t currentHue = 0;
    bool reverse = false;
};