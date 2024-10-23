#include "LedUtils.h"
#include "ColorUtils.h"

namespace LedUtils {
    void setSolidColor(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip, uint32_t color) {
        RgbColor rgbColor = ColorUtils::colorToRgbColor(color);
        ledStrip.ClearTo(rgbColor);
    }


    int getAppropriateLedEffectPixel(int pixel) {
        if (strcmp(PIOENV, "desk") == 0) {
            return getFoldedPixelIndex(pixel);
        }
        return pixel;
    }

    int getFoldedPixelIndex(int pixel) {
        int actualPixel = pixel;
        int pixelPlusOne = pixel + 1;
        if (pixelPlusOne > LED_PIXEL_COUNT / 2)
            actualPixel = LED_PIXEL_COUNT - pixelPlusOne;
        return actualPixel;
    }


}