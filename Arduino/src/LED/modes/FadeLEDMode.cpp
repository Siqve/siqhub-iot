#include "Arduino.h"
#include "FadeLEDMode.h"

#include <utility>
#include "utilities/ColorUtils.h"

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */
#define DEFAULT_BASE_FPS 200
#define DEFAULT_SPEED 25   /* Controls the HUE increments per cycle */
#define DEFAULT_PIXEL_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/

FadeLEDMode::FadeLEDMode(std::shared_ptr<NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>>& LEDStripPtr,
                         std::function<void(int)> setFPS) : LEDMode(LEDStripPtr, std::move(setFPS)),
                                                            baseFPS(DEFAULT_BASE_FPS),
                                                            speed(DEFAULT_SPEED),
                                                            pixelColorHop(DEFAULT_PIXEL_COLOR_HOP) {
}

void FadeLEDMode::onActivate() {
    this->setFPS(DEFAULT_BASE_FPS);
}

void FadeLEDMode::loop() {
    cycleFade();
    LEDStripPtr->Show();
}

void FadeLEDMode::onUpdate(AsyncWebServerRequest *request) {
    if (request->hasParam("fps")) {
        int val = request->getParam("fps")->value().toInt();
        setFPS(val);
        baseFPS = val;

    }
    if (request->hasParam("speed")) {
        int val = request->getParam("speed")->value().toInt();
        speed = val;
    }
    if (request->hasParam("hop")) {
        int val = request->getParam("hop")->value().toInt();
        pixelColorHop = val;
    }
    if (request->hasParam("reverse")) {
        int val = request->getParam("reverse")->value().toInt();
        reverse = val;
    }
}

void FadeLEDMode::cycleFade() {
    incrementHue();
    for (int i = 0; i < LEDStripPtr->PixelCount(); i++) {
    RgbColor color = ColorUtils::HSVToRgbColor(currentHue + i * pixelColorHop, SATURATION, BRIGHTNESS);
        LEDStripPtr->SetPixelColor(i, color);
    }
}

void FadeLEDMode::incrementHue() {
    //Decreasing hue will lead to effect coming from "source" outwards
    currentHue -= reverse ? speed : -speed;
}

/**
 * Format: FPS, SPEED, PIXEL_HOP, REVERSE
 */
String FadeLEDMode::getSettings() {
    return String(baseFPS) + "," + String(speed) + "," + String(pixelColorHop) + "," + reverse;
}