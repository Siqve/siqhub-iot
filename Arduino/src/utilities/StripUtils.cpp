#include "StripUtils.h"

namespace StripUtils {
    void setSolidColor(const std::shared_ptr<Adafruit_NeoPixel>& LEDStripPtr, int pixelCount, uint32_t color) {
        for (int i = 0; i < pixelCount; i++) {
            LEDStripPtr->setPixelColor(i, color);
        }
    }
}