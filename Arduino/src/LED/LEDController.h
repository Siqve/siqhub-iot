#ifndef ARDUINO_LEDCONTROLLER_H
#define ARDUINO_LEDCONTROLLER_H

#include "LEDMode.h"
#include "ESPAsyncWebServer.h"
#include "Adafruit_NeoPixel.h"
#include "NeoPixelBus.h"


class LEDController {
public:
    LEDController();
    void setup(int ledDataPin);
    void loop();
    void incomingUpdate(AsyncWebServerRequest *request);
    void incomingDebug(AsyncWebServerRequest *request);
    int getActiveModeNumber();
    std::shared_ptr<LEDMode> getActiveMode();
    String getModeAndSettings();

    int getLEDStripPixelCount() {
        return LEDStripPtr->PixelCount();
    }


private:
    int activeFPS;
    std::vector<std::shared_ptr<LEDMode>> modes;
    std::shared_ptr<NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>> LEDStripPtr;
    int activeModeNumber = 0;

    void initEffects();
    void loop_LED(unsigned long timeNow);
    void readPhysicalDebugButton();
    void setFPS(int newFPS);


    void debugButtonClick();
    void loop_physicalDebugButton(unsigned long timeNow);
};

#endif //ARDUINO_LEDCONTROLLER_H
