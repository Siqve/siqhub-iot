#ifndef ARDUINO_LEDUTILS_H
#define ARDUINO_LEDUTILS_H

#include <memory>
#include "NeoPixelBus.h"

namespace LEDUtils {
    extern void setSolidColor(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, uint32_t color);
    extern int getAppropriateLedEffectPixel(int pixel);
    extern int getFoldedPixelIndex(int pixel);
};


#endif //ARDUINO_LEDUTILS_H
