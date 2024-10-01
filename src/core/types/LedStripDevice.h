#pragma once

#include <memory>
#include "ArduinoJson.h"
#include "BaseDevice.h"
#include "hardware/LED/modes/LEDMode.h"

class LedStripDevice : public BaseDevice {
public:
    explicit LedStripDevice();
    void loop() override;
    void updateSettings(const JsonDocument& settings) override;
    void setup(const JsonDocument& settings) override;
private:
    std::shared_ptr<LEDMode> getActiveMode();
    void initEffects();

    void setFPS(int newFPS);

    int activeFps = 1;
    int activeModeNumber = 0;

    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> ledStrip = NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(LED_PIXEL_COUNT, 5);
    std::vector<std::shared_ptr<LEDMode>> modes;
    Debug::Logger logger = Debug::Logger("LedStripDevice");

};
