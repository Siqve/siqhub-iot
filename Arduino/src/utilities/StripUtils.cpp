#include "StripUtils.h"
#include "ColorUtils.h"

namespace StripUtils {
    void setSolidColor(const std::shared_ptr<NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>>& LEDStripPtr, uint32_t color) {
        RgbColor rgbColor = ColorUtils::colorToRgbColor(color);
        for (int i = 0; i < LEDStripPtr->PixelCount(); i++)
            LEDStripPtr->SetPixelColor(i, rgbColor);

    }
}