#pragma once

#include <memory>
#include "hardware/led/constants/LedSettingsConstants.h"

#include "ArduinoJson.h"
#include "core/BaseDevice.h"
#include "NeoPixelBus.h"
#include "debug/Logger.h"


class LedStripDevice : public BaseDevice {
public:
    explicit LedStripDevice();
    void loop() override;
    void handleSettingsUpdate(const JsonDocument &settings) override;
    void initialize(const JsonDocument &settings) override;
    int getFps() override;
private:
    void singleColorLoop();
    void gradientLoop();

    void reload();

    void handleColorProfileUpdate(const JsonVariantConst &colorRow);
    void createColorProfileListener();
    void removeListener();

    void enableDebugCommands();

    JsonDocument getInitialColorProfile();

    bool isSingleColor = false;
    std::vector<uint32_t> colors;
    int currentBaseColorIndex = 0;

    // Settings
    std::string colorProfileId;
    int fps = 1;
    float frequency = 1;
    bool reverse = false;

    boolean listenerConnected = false;

    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> ledStrip = NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(LED_PIXEL_COUNT, 4);
    Debug::Logger logger = Debug::Logger("LedStripDevice");

};
