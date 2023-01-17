#ifndef BPMEffect_h
#define BPMEffect_h

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "../LEDMode.h"

class FadeLEDMode : public LEDMode {
  public:
    FadeLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, int pixelCount, std::function<void(int)> setFPS);
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
#endif
