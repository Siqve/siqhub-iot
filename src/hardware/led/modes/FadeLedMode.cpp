#include "FadeLedMode.h"

#include "utils/ColorUtils.h"
#include "hardware/led/utils/LedUtils.h"

#define DEFAULT_BRIGHTNESS 255
#define SATURATION 255
#define DEFAULT_BASE_FPS 200
#define DEFAULT_SPEED 25   /* Controls the HUE increments per cycle */
#define DEFAULT_PIXEL_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/

FadeLedMode::FadeLedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> &ledStrip,
                         std::function<void(int)> setFps) : LedMode(ledStrip, Debug::Logger("FadeLedMode"), std::move(setFps)),
                                                            ledFps(DEFAULT_BASE_FPS),
                                                            ledSpeed(DEFAULT_SPEED),
                                                            ledPixelHueStep(DEFAULT_PIXEL_COLOR_HOP),
                                                            ledBrightness(DEFAULT_BRIGHTNESS) {
}


void FadeLedMode::loop() {
    cycleFade();
    ledStrip.Show();
}

void FadeLedMode::handleUpdate(const JsonDocument &settings) {
}

void FadeLedMode::cycleFade() {
    incrementHue();
    for (int i = 0; i < ledStrip.PixelCount(); i++) {
        int ledEffectPixel = LedUtils::getAppropriateLedEffectPixel(i);
        RgbColor color = ColorUtils::HSVToRgbColor((ledPixelHueStep * ledEffectPixel) + currentHue, SATURATION,
                                                   ledBrightness);
        ledStrip.SetPixelColor(i, color);
    }
}

void FadeLedMode::incrementHue() {
    //Decreasing hue will lead to effect coming from "source" outwards
    currentHue += reverse ? -ledSpeed : ledSpeed;
}
