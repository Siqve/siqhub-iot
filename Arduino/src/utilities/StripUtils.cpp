#include "StripUtils.h"

namespace StripUtils {
    void setSolidColor(const std::shared_ptr<Adafruit_NeoPixel>& LEDStripPtr, uint32_t color) {
        for (int i = 0; i < LEDStripPtr->numPixels(); i++) {
            LEDStripPtr->setPixelColor(i, color);
        }
    }
}