#include "LEDUtils.h"
#include "ColorUtils.h"
#include "Constants.h"

namespace LEDUtils {
    void setSolidColor(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, uint32_t color) {
        RgbColor rgbColor = ColorUtils::colorToRgbColor(color);
        LEDStripPtr.ClearTo(rgbColor);
//        for (int i = 0; i < LEDStripPtr.PixelCount(); i++)
//            LEDStripPtr.SetPixelColor(i, rgbColor);
    }

    int getFoldedPixelIndex(int pixel) {
        int actualPixel = pixel;
        int pixelPlusOne = pixel + 1;
        if (pixelPlusOne > LED_PIXEL_COUNT / 2)
            actualPixel = LED_PIXEL_COUNT - pixelPlusOne;
        return actualPixel;
    }


}