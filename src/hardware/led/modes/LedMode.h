#pragma once

#include "NeoPixelBus.h"
#include "debug/Logger.h"
#include "networking/web/RequestWrapper.h"
#include "ArduinoJson.h"

class LedMode {
public:
    virtual ~LedMode() = default;

    virtual void loop() = 0;

    virtual void handleUpdate(const JsonDocument& settings) = 0;

    virtual void onDebugCommand(const std::string& command) {};

protected:
    explicit LedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip, Debug::Logger logger,
                     std::function<void(int)> setFps) : ledStrip(ledStrip), logger(std::move(logger)) {
        this->setFps = std::move(setFps);
    }
    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip;
    Debug::Logger logger;
    std::function<void(int)> setFps{};
};