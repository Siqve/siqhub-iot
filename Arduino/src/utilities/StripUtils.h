#ifndef ARDUINO_STRIPUTILS_H
#define ARDUINO_STRIPUTILS_H

#include <memory>
#include "Adafruit_NeoPixel.h"

namespace StripUtils {
    extern void setSolidColor(const std::shared_ptr<Adafruit_NeoPixel>& LEDStripPtr, int pixelCount, uint32_t color);
};


#endif //ARDUINO_STRIPUTILS_H
