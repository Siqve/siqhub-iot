#include "MusicLEDMode.h"
#include "utilities/ColorUtils.h"

#include <utility>

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */

#define DEFAULT_BASE_FPS 20
#define DEFAULT_SPEED 25   /* Controls the HUE increments per cycle */
#define DEFAULT_PIXEL_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/
#define DEFAULT_BOUNCE 16000

MusicLEDMode::MusicLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr,
                           std::function<void(int)> setFPS) : LEDMode(std::move(LEDStripPtr)),
                                                              baseFPS(DEFAULT_BASE_FPS), speed(DEFAULT_SPEED),
                                                              pixelColorHop(DEFAULT_PIXEL_COLOR_HOP),
                                                              bounce(DEFAULT_BOUNCE) {
    this->setFPS = std::move(setFPS);
}

uint16_t hue = 0;

unsigned long lastBeat = millis();

void MusicLEDMode::onActivate() {
    this->setFPS(DEFAULT_BASE_FPS);
}


void MusicLEDMode::loop() {
    cycleFade();
    updateFPS();
    LEDStripPtr->show();
}


void MusicLEDMode::cycleFade() {
    incrementHue();
    for (int i = 0; i < LEDStripPtr->numPixels(); i++) {
        uint32_t color = ColorUtils::HSVToColor(hue + i * pixelColorHop, SATURATION, BRIGHTNESS);
        LEDStripPtr->setPixelColor(i, color);
    }
}

void MusicLEDMode::updateFPS() {
    setFPS(baseFPS + getSpeedBoost());
}


void MusicLEDMode::beat() {
    speedBoost = static_cast<float> (bounce);
}

void MusicLEDMode::onUpdate(AsyncWebServerRequest *request) {
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
    if (request->hasParam("bounce")) {
        int val = request->getParam("bounce")->value().toInt();
        bounce = val;
    }
}


float MusicLEDMode::getSpeedBoost() {
    return stabilizeSpeed();
}

float MusicLEDMode::stabilizeSpeed() {
    return speedBoost = max(speedBoost - pow(speedBoost/(10.2), 1.02)*0.1 - 10, 0.0);
}


void MusicLEDMode::incrementHue() {
    //Decreasing hue will lead to effect coming from "source" outwards
    hue -= !reverse ? speed : -speed;
}

void MusicLEDMode::debugButtonClick() {
    beat();
}

/**
 * Format: FPS, SPEED, PIXEL_HOP, REVERSE
 */
String MusicLEDMode::getSettings() {
    return String(baseFPS) + "," + String(speed) + "," + String(pixelColorHop) + "," + reverse + "," + String(bounce);
}
