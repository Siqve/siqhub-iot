#pragma once

#include "LEDMode.h"
#include "ArduinoJson.h"

class StaticLEDMode : public LEDMode {
public:
    StaticLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStrip, std::function<void(int)> setFPS);
    void onActivate(const JsonDocument& settings) override;
    void loop() override;
    void onUpdate(const RequestWrapper& request) override;
    void onDebugCommand(const std::string& command) override;
private:
    uint32_t staticColor = 16711680; // Default static color, red
};