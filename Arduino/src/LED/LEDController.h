#ifndef ARDUINO_LEDCONTROLLER_H
#define ARDUINO_LEDCONTROLLER_H

#include "LEDMode.h"
#include "ESPAsyncWebServer.h"
#include "Adafruit_NeoPixel.h"
#include "NeoPixelBus.h"
#include "utilities/DebugManager.h"
#include "Constants.h"


class LEDController {
public:
    LEDController() : LEDStripPtr(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>(LEDConstants::LED_PIXEL_COUNT)),
                      logger(DebugManager::getInstance().newLogger("LEDController")) {};
    void setup();
    void loop();
    void incomingUpdate(AsyncWebServerRequest* request);
    void incomingDebug(AsyncWebServerRequest* request);
    int getActiveModeNumber();
    std::shared_ptr<LEDMode> getActiveMode();
    String getModeAndSettings();

    int getLEDStripPixelCount() {
        return LEDStripPtr.PixelCount();
    }


private:
    NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> LEDStripPtr;
    std::vector<std::shared_ptr<LEDMode>> modes;

    DebugManager::Logger logger;

    int activeModeNumber = 0;
    int activeFPS = 1;

    void initEffects();
    void loop_LED(unsigned long timeNow);
    void setFPS(int newFPS);
    void debugButtonClick();
};

#endif //ARDUINO_LEDCONTROLLER_H
