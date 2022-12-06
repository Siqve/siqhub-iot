#ifndef BPMEffect_h
#define BPMEffect_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class BPMEffect {
  public:
    BPMEffect(Adafruit_NeoPixel strip, int pixel_count, void *fpsFunction);
    void loop();
    void testButtonClick();
  private:
    Adafruit_NeoPixel STRIP;
    int PIXEL_COUNT;
    void (*setFPS)(int);
    void cycleFade();
    void beat();
    void incrementHue(bool);
    void updateFPS();
    float getSpeedBoost();
    float stabilizeSpeed();
};
#endif
