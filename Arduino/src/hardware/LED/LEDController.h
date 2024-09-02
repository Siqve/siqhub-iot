#pragma once

#include "LEDMode.h"
#include "ESPAsyncWebServer.h"
#include "web/RequestWrapper.h"


class LEDController {
public:
    LEDController() : LEDStripPtr(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(LED_PIXEL_COUNT, 5)),
                      logger(Debug::Logger("LEDController")) {};
    void setup();
    void loop();
    void incomingDebug();
    int getActiveModeNumber();
    std::shared_ptr<LEDMode> getActiveMode();
    String getModeAndSettings();

    int getLEDStripPixelCount() {
        return LEDStripPtr.PixelCount();
    }


private:
    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> LEDStripPtr;
    std::vector<std::shared_ptr<LEDMode>> modes;

    Debug::Logger logger;

    int activeModeNumber = 0;
    int activeFPS = 1;

    void initEffects();
    void loop_LED(unsigned long timeNow);
    void setFPS(int newFPS);
    void debugButtonClick();
    AsyncWebServerResponse* onUpdate(const RequestWrapper& request);
};