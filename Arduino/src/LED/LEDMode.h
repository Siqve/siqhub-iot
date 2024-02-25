#ifndef ARDUINO_LEDMODE_H
#define ARDUINO_LEDMODE_H

#include <utility>
#include <memory>
#include "ESPAsyncWebServer.h"
#include "NeoPixelBus.h"
#include "utilities/DebugManager.h"

class LEDMode {
public:
    virtual ~LEDMode() = default;

    virtual void loop() = 0;

    virtual void onActivate() {};
    virtual void debugButtonClick() {};

    virtual void onUpdate(AsyncWebServerRequest* request) {};
    virtual void onDebugCommand(const std::string& command) {};

    virtual String getSettings() {
        return {};
    };
protected:
    explicit LEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr,
                     std::function<void(int)> setFPS) : LEDStripPtr(LEDStripPtr), logger(DebugManager::getInstance().newLogger("LEDController")) {
        this->setFPS = std::move(setFPS);
    }
    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr;
    DebugManager::Logger logger;
    std::function<void(int)> setFPS{};
};


#endif //ARDUINO_LEDMODE_H
