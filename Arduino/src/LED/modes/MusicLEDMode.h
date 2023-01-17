#ifndef ARDUINO_MUSICLEDMODE_H
#define ARDUINO_MUSICLEDMODE_H


#include "LED/LEDMode.h"

class MusicLEDMode : public LEDMode {
public:
    MusicLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, int pixelCount, std::function<void(int)> setFPS);
    void onActivate() override;
    void loop() override;
    void debugButtonClick() override;
private:
    void cycleFade();
    void beat();
    void incrementHue(bool);
    void updateFPS();
    float getSpeedBoost();
    float stabilizeSpeed();

};

#endif //ARDUINO_MUSICLEDMODE_H
