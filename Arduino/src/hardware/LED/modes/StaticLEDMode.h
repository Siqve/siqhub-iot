#pragma once

#include "hardware/LED/LEDMode.h"
#include "Constants.h"

class StaticLEDMode : public LEDMode {
public:
    StaticLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStrip, std::function<void(int)> setFPS);
    void onActivate() override;
    void loop() override;
    void onUpdate(const RequestWrapper& request) override;
    void onDebugCommand(const std::string& command) override;
private:
    uint32_t staticColor = LEDConstants::DEFAULT_STATIC_COLOR;
};