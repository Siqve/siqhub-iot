#include "Arduino.h"
#include "FadeLEDMode.h"

#include <utility>
#include "utilities/ColorUtils.h"

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */
#define SPEED 25   /* Control the saturation of your leds */
#define LED_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/
#define BASE_FPS 200

FadeLEDMode::FadeLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, int pixelCount,
                         std::function<void(int)> setFPS) : LEDMode(std::move(LEDStripPtr), pixelCount) {
    this->setFPS = std::move(setFPS);
}

float speedBoost = 0;
uint16_t hue = 0;

unsigned long lastBeat = millis();

void FadeLEDMode::onActivate() {
}


void FadeLEDMode::loop() {
    cycleFade();
    updateFPS();
    LEDStripPtr->show();
}


void FadeLEDMode::cycleFade() {
    incrementHue(true);
    for (int i = 0; i < LEDStripPixelCount; i++) {
        uint32_t color = ColorUtils::HSVToColor(hue + i * LED_COLOR_HOP, SATURATION, BRIGHTNESS);
        LEDStripPtr->setPixelColor(i, color);
    }
}

void FadeLEDMode::updateFPS() {
    setFPS(BASE_FPS + getSpeedBoost());
}


void FadeLEDMode::beat() {
    speedBoost = 16000;
}


float FadeLEDMode::getSpeedBoost() {
    return stabilizeSpeed();
}

float FadeLEDMode::stabilizeSpeed() {
    return speedBoost = max(speedBoost - 0.000005 * pow(speedBoost, 2), 0.0);
}


void FadeLEDMode::incrementHue(bool reverse) {
    //Decreasing hue will lead to effect coming from "source" outwards
    hue -= !reverse ? SPEED : -SPEED;
}

void FadeLEDMode::debugButtonClick() {
    Serial.println("Test");
    beat();
}
