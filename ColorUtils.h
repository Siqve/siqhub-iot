#ifndef ColorUtils_h
#define ColorUtils_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class ColorUtils {
  public:
    static uint32_t HSVToColor(uint16_t hue, uint8_t sat, uint8_t val);
    static uint32_t Gamma32(uint32_t x);
    static uint8_t Gamma8(uint8_t x);
    static uint32_t rgbToColor(uint8_t r, uint8_t g, uint8_t b);
    static int * colorToRGB(uint32_t color);
};

#endif
