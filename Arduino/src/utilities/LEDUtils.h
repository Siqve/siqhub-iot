#ifndef ARDUINO_LEDUTILS_H
#define ARDUINO_LEDUTILS_H

#include <memory>
#include "Adafruit_NeoPixel.h"
#include "NeoPixelBus.h"

namespace LEDUtils {
    extern void setSolidColor(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, uint32_t color);
    extern int getFoldedPixelIndex(int pixel);
};


#endif //ARDUINO_LEDUTILS_H
