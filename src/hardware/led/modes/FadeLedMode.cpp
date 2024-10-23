#include "FadeLedMode.h"

#include "utils/ColorUtils.h"
#include "utils/ledUtils.h"

#define DEFAULT_BRIGHTNESS 255
#define SATURATION 255
#define DEFAULT_BASE_FPS 200
#define DEFAULT_SPEED 25   /* Controls the HUE increments per cycle */
#define DEFAULT_PIXEL_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/

FadeLedMode::FadeLedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip,
                         std::function<void(int)> setFPS) : LedMode(ledStrip, Debug::Logger("FadeLedMode"), std::move(setFPS)),
                                                            ledFPS(DEFAULT_BASE_FPS),
                                                            ledSpeed(DEFAULT_SPEED),
                                                            ledPixelHueStep(DEFAULT_PIXEL_COLOR_HOP),
                                                            ledBrightness(DEFAULT_BRIGHTNESS) {
}

void FadeLedMode::initialize(const JsonDocument& settings) {
    setFPS(DEFAULT_BASE_FPS);
}

void FadeLedMode::loop() {
    cycleFade();
    ledStrip.Show();
}

void FadeLedMode::updateFps() {
    setFPS(ledFPS);
}

void FadeLedMode::onUpdate(const RequestWrapper& request) {
    if (request.hasParam("fps")) {
        int val = request.getParam("fps")->value().toInt();
        ledFPS = val;
        updateFps();
    }
    if (request.hasParam("ledSpeed")) {
        int val = request.getParam("ledSpeed")->value().toInt();
        ledSpeed = val;
    }
    if (request.hasParam("hop")) {
        int val = request.getParam("hop")->value().toInt();
        ledPixelHueStep = val;
    }
    if (request.hasParam("brightness")) {
        int val = request.getParam("brightness")->value().toInt();
        ledBrightness = val;
    }
    if (request.hasParam("reverse")) {
        int val = request.getParam("reverse")->value().toInt();
        reverse = val;
    }
}

void FadeLedMode::cycleFade() {
    incrementHue();
    for (int i = 0; i < ledStrip.PixelCount(); i++) {
        int ledEffectPixel = ledUtils::getAppropriateLedEffectPixel(i);
        RgbColor color = ColorUtils::HSVToRgbColor((ledPixelHueStep * ledEffectPixel) + currentHue, SATURATION,
                                                   ledBrightness);
        ledStrip.SetPixelColor(i, color);
    }
}

void FadeLedMode::incrementHue() {
    //Decreasing hue will lead to effect coming from "source" outwards
    currentHue += reverse ? -ledSpeed : ledSpeed;
}

/**
 * Format: FPS, SPEED, PIXEL_HOP, REVERSE
 */
String FadeLedMode::getSettings() {
    return String(ledFPS) + "," + String(ledSpeed) + "," + String(ledPixelHueStep) + "," + String(ledBrightness) + "," + reverse;
}

String FadeLedMode::getSettingsJSON() {
    return String("{") +
           "\"ledFPS\": " + ledFPS +
           ", \"ledSpeed\": " + ledSpeed +
           ", \"ledPixelHueStep\": " + ledPixelHueStep +
           ", \"ledBrightness\": " + ledBrightness +
           ", \"reverse\": " + reverse
           + String("}");
}
