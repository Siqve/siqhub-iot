#ifndef ARDUINO_LEDCONTROLLER_H
#define ARDUINO_LEDCONTROLLER_H



#include "LEDMode.h"

typedef std::shared_ptr<LEDMode> LEDModePtr;

class LEDController {
public:
    LEDController();
    void setup();
    void loop();
    void incomingUpdate(AsyncWebServerRequest *request);
    int getActiveMode();
private:
    int activeFPS;
    std::vector <LEDModePtr> modes;
    Adafruit_NeoPixel LEDStrip;

    int activeMode = 0;
    void initEffects();

    void loop_LED(unsigned long timeNow);
    void loop_testButton(unsigned long timeNow);
    void readTestButton();
    void testButtonClick();
    void setFPS(int newFPS);
};


#endif //ARDUINO_LEDCONTROLLER_H
