#ifndef ARDUINO_LEDCONTROLLER_H
#define ARDUINO_LEDCONTROLLER_H


#include "LEDMode.h"

class LEDController {
public:
    LEDController();
    void setup();
    void loop();
    void incomingUpdate(AsyncWebServerRequest *request);
    void incomingDebug(AsyncWebServerRequest *request);
    int getActiveModeNumber();
    std::shared_ptr<LEDMode> getActiveMode();
private:
    int activeFPS;
    std::vector<std::shared_ptr<LEDMode>> modes;
    std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr;

    int activeModeNumber = 0;
    void initEffects();

    void loop_LED(unsigned long timeNow);

    void loop_physicalDebugButton(unsigned long timeNow);
    void readPhysicalDebugButton();
    void debugButtonClick();
    void setFPS(int newFPS);
};

#endif //ARDUINO_LEDCONTROLLER_H
