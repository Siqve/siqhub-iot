#pragma once

#include "NeoPixelBus.h"

namespace LEDUtils {
    void setSolidColor(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, uint32_t color);
    int getAppropriateLedEffectPixel(int pixel);
    int getFoldedPixelIndex(int pixel);
};
