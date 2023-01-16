#include "Arduino.h"
#include "PulsatingLEDMode.h"

#include <utility>
#include "utilities/ColorUtils.h"

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */
#define SPEED 25   /* Control the saturation of your leds */
#define LED_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/
#define BASE_FPS 2

PulsatingLEDMode::PulsatingLEDMode(Adafruit_NeoPixel *LEDStrip, int pixelCount, std::function<void(int)> setFPS) {
    this->LEDStrip = *LEDStrip;
    LEDStripPixelCount = pixelCount;
    this->setFPS = std::move(setFPS);
}

float speedBoost = 0;
uint16_t hue = 0;

unsigned long lastBeat = millis();

void PulsatingLEDMode::loop() {
    cycleFade();
    updateFPS();
    LEDStrip.show();
}


void PulsatingLEDMode::cycleFade() {
    incrementHue(true);
    for (int i = 0; i < LEDStripPixelCount; i++) {
        uint32_t color = ColorUtils::HSVToColor(hue + i * LED_COLOR_HOP, SATURATION, BRIGHTNESS);
        LEDStrip.setPixelColor(i, color);
    }
}

void PulsatingLEDMode::updateFPS() {
    setFPS(BASE_FPS + getSpeedBoost());
}


void PulsatingLEDMode::beat() {
    speedBoost = 16000;
}


float PulsatingLEDMode::getSpeedBoost() {
    return stabilizeSpeed();
}

float PulsatingLEDMode::stabilizeSpeed() {
    return speedBoost = max(speedBoost - 0.000005 * pow(speedBoost, 2), 0.0);
}


void PulsatingLEDMode::incrementHue(bool reverse) {
    //Decreasing hue will lead to effect coming from "source" outwards
    hue -= !reverse ? SPEED : -SPEED;
}

void PulsatingLEDMode::testButtonClick() {
    Serial.println("Test");
    beat();
}