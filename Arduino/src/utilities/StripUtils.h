#ifndef ARDUINO_STRIPUTILS_H
#define ARDUINO_STRIPUTILS_H

#include <memory>
#include "Adafruit_NeoPixel.h"
#include "NeoPixelBus.h"

namespace StripUtils {
    extern void setSolidColor(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, uint32_t color);
};


#endif //ARDUINO_STRIPUTILS_H
