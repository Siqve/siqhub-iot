#pragma once

#include "hardware/LED/modes/LEDMode.h"
#include "ESPAsyncWebServer.h"
#include "networking/web/RequestWrapper.h"


class LEDController {
public:
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
    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> LEDStripPtr = NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(LED_PIXEL_COUNT, 5);
    std::vector<std::shared_ptr<LEDMode>> modes;

    Debug::Logger logger = Debug::Logger("LEDController");

    int activeModeNumber = 0;
    int activeFPS = 1;

    void initEffects();
    void loop_LED(unsigned long timeNow);
    void setFPS(int newFPS);
    void debugButtonClick();
    AsyncWebServerResponse* onUpdate(const RequestWrapper& request);
};