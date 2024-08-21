#pragma once

#include "NeoPixelBus.h"
#include "debug/Logger.h"
#include "web/RequestWrapper.h"

class LEDMode {
public:
    virtual ~LEDMode() = default;

    virtual void loop() = 0;

    virtual void onActivate() {};
    virtual void debugButtonClick() {};

    virtual void onUpdate(const RequestWrapper& request) {};
    virtual void onDebugCommand(const std::string& command) {};

    virtual String getSettings() {
        return {};
    };
protected:
    explicit LEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, Debug::Logger logger,
                     std::function<void(int)> setFPS) : LEDStripPtr(LEDStripPtr), logger(std::move(logger)) {
        this->setFPS = std::move(setFPS);
    }
    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr;
    Debug::Logger logger;
    std::function<void(int)> setFPS{};
};