#ifndef BPMEffect_h
#define BPMEffect_h

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "../LEDMode.h"

class PulsatingLEDMode : public LEDMode {
  public:
    PulsatingLEDMode(Adafruit_NeoPixel* LEDStrip, int pixelCount, std::function<void(int)> setFPS);
    void loop() override;
    void testButtonClick() override;
  private:
    void cycleFade();
    void beat();
    void incrementHue(bool);
    void updateFPS();
    float getSpeedBoost();
    float stabilizeSpeed();
};
#endif
