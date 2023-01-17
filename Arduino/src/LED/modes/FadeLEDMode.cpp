#include "Arduino.h"
#include "FadeLEDMode.h"

#include <utility>
#include "utilities/ColorUtils.h"

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */
#define SPEED 25   /* Controls the HUE increments per cycle */
#define LED_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/
#define BASE_FPS 200

FadeLEDMode::FadeLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, int pixelCount,
                         std::function<void(int)> setFPS) : LEDMode(std::move(LEDStripPtr), pixelCount) {
    this->setFPS = std::move(setFPS);
    this->setFPS(BASE_FPS);
}

void FadeLEDMode::loop() {
    cycleFade();
    LEDStripPtr->show();
}

void FadeLEDMode::onUpdate(AsyncWebServerRequest *request) {
    if (request->hasParam("fps")) {
        //TODO
    }
    if (request->hasParam("reverse")) {
        //TODO
    }
}

void FadeLEDMode::cycleFade() {
    incrementHue();
    for (int i = 0; i < LEDStripPixelCount; i++) {
        uint32_t color = ColorUtils::HSVToColor(currentHue + i * LED_COLOR_HOP, SATURATION, BRIGHTNESS);
        LEDStripPtr->setPixelColor(i, color);
    }
}

void FadeLEDMode::incrementHue() {
    //Decreasing hue will lead to effect coming from "source" outwards
    currentHue -= reverse ? SPEED : -SPEED;
}
