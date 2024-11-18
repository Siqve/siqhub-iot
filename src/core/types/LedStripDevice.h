#pragma once

#include <memory>
#include <constants/LedConstants.h>

#include "ArduinoJson.h"
#include "BaseDevice.h"
#include "NeoPixelBus.h"
#include "debug/Logger.h"


class LedStripDevice : public BaseDevice {
public:
    explicit LedStripDevice();
    void loop() override;
    void updateSettings(const JsonDocument& settings) override;
    void initialize(const JsonDocument& settings) override;
    int getFps() override;
private:
    void singleColorLoop();
    void gradientLoop();
    void createStaticListener(const JsonDocument& settings);
    JsonDocument getInitialStaticSettings(const JsonDocument& settings);
    void handleColorProfileUpdate(const JsonVariantConst& colorRow);

    bool isSingleColor = false;

    int fps = 1;

    std::vector<uint32_t> colors;

    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> ledStrip = NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(LED_PIXEL_COUNT, 4);

    Debug::Logger logger = Debug::Logger("LedStripDevice");

};
