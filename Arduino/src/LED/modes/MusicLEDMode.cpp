#include "MusicLEDMode.h"
#include "utilities/ColorUtils.h"

#include <utility>

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */
#define SPEED 25   /* Controls the HUE increments per cycle */
#define LED_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/
#define BASE_FPS 200

MusicLEDMode::MusicLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, int pixelCount,
                           std::function<void(int)> setFPS) : LEDMode(std::move(LEDStripPtr), pixelCount) {
    this->setFPS = std::move(setFPS);
}

float speedBoost = 0;
uint16_t hue = 0;

unsigned long lastBeat = millis();

void MusicLEDMode::onActivate() {
}


void MusicLEDMode::loop() {
    cycleFade();
    updateFPS();
    LEDStripPtr->show();
}


void MusicLEDMode::cycleFade() {
    incrementHue(true);
    for (int i = 0; i < LEDStripPixelCount; i++) {
        uint32_t color = ColorUtils::HSVToColor(hue + i * LED_COLOR_HOP, SATURATION, BRIGHTNESS);
        LEDStripPtr->setPixelColor(i, color);
    }
}

void MusicLEDMode::updateFPS() {
    setFPS(BASE_FPS + getSpeedBoost());
}


void MusicLEDMode::beat() {
    speedBoost = 16000;
}


float MusicLEDMode::getSpeedBoost() {
    return stabilizeSpeed();
}

float MusicLEDMode::stabilizeSpeed() {
    return speedBoost = max(speedBoost - 0.000005 * pow(speedBoost, 2), 0.0);
}


void MusicLEDMode::incrementHue(bool reverse) {
    //Decreasing hue will lead to effect coming from "source" outwards
    hue -= !reverse ? SPEED : -SPEED;
}

void MusicLEDMode::debugButtonClick() {
    Serial.println("Test");
    beat();
}

