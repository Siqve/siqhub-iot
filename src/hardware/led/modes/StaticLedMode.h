#pragma once

#include "LedMode.h"
#include "ArduinoJson.h"

class StaticLedMode : public LedMode {
public:
    StaticLedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip, std::function<void(int)> setFps);
    void loop() override;
    void handleUpdate(const JsonDocument& settings) override;
    void onDebugCommand(const std::string& command) override;
private:
    uint32_t staticColor = 16711680; // Default static color, red
};