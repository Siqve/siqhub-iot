#pragma once

#include "NeoPixelBus.h"

namespace LedUtils {
    void setSolidColor(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> &ledStrip, uint32_t color);
    int getAppropriateLedEffectPixel(int pixel);
    int getFoldedPixelIndex(int pixel);
};
