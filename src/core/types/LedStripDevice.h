#pragma once

#include <memory>
#include <constants/LedConstants.h>

#include "ArduinoJson.h"
#include "BaseDevice.h"
#include "hardware/led/modes/LedMode.h"

class LedStripDevice : public BaseDevice {
public:
    explicit LedStripDevice();
    void loop() override;
    void updateSettings(const JsonDocument& settings) override;
    void initialize(const JsonDocument& settings) override;
private:
    std::shared_ptr<LedMode> getMode();
    void initEffects();

    void setFps(int newFps);
    int activeFps = 1;

    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> ledStrip = NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(LED_PIXEL_COUNT, 5);

    LedConstants::LedModeType::Value activeModeType = LedConstants::LedModeType::Value::UNKNOWN;
    std::unordered_map<LedConstants::LedModeType::Value, std::shared_ptr<LedMode>> ledModes;

    Debug::Logger logger = Debug::Logger("LedStripDevice");

};
