#ifndef BPMEffect_h
#define BPMEffect_h

#include "../LEDMode.h"

class FadeLEDMode : public LEDMode {
  public:
    FadeLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, std::function<void(int)> setFPS);
    void loop() override;
    void onUpdate(AsyncWebServerRequest *request) override;
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
#endif
