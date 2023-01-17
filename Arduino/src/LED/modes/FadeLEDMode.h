#ifndef BPMEffect_h
#define BPMEffect_h

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "../LEDMode.h"

class FadeLEDMode : public LEDMode {
  public:
    FadeLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, int pixelCount, std::function<void(int)> setFPS);
    void loop() override;
    void onUpdate(AsyncWebServerRequest *request) override;
  private:
    void cycleFade();
    void incrementHue();
    uint16_t currentHue = 0;
    bool reverse = false;
};
#endif
