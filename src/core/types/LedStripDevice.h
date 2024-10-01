#pragma once

#include <memory>
#include "ArduinoJson.h"
#include "BaseDevice.h"
#include "hardware/LED/modes/LEDMode.h"

class LedStripDevice : public BaseDevice {
public:
    explicit LedStripDevice();
    void loop() override;
    void applySettings(const JsonDocument& settings) override;
private:
    std::vector<std::shared_ptr<LEDMode>> modes;
    Debug::Logger logger = Debug::Logger("LedStripDevice");

};
