#include "MusicLEDMode.h"
#include "utilities/ColorUtils.h"
#include "utilities/LEDUtils.h"

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */

#define DEFAULT_BASE_FPS 20
#define DEFAULT_SPEED 150   /* Controls the HUE increments per cycle */
#define DEFAULT_PIXEL_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/
#define DEFAULT_BOUNCE 500000

MusicLEDMode::MusicLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr,
                           std::function<void(int)> setFPS) : LEDMode(LEDStripPtr, std::move(setFPS)),
                                                              baseFPS(DEFAULT_BASE_FPS), speed(DEFAULT_SPEED),
                                                              pixelColorHop(DEFAULT_PIXEL_COLOR_HOP),
                                                              bounce(DEFAULT_BOUNCE) {
}

void MusicLEDMode::onActivate() {
    this->setFPS(DEFAULT_BASE_FPS);
}


void MusicLEDMode::loop() {
    cycleFade();
    updateFPS();
    LEDStripPtr.Show();
}


void MusicLEDMode::cycleFade() {
    incrementHue();
    for (int i = 0; i < LEDStripPtr.PixelCount(); i++) {
        int ledEffectPixel = LEDUtils::getFoldedPixelIndex(i);
        RgbColor color = ColorUtils::HSVToRgbColor(currentHue + pixelColorHop * ledEffectPixel, SATURATION, BRIGHTNESS);
        LEDStripPtr.SetPixelColor(i, color);
    }
}

void MusicLEDMode::updateFPS() {
    setFPS(baseFPS + getSpeedBoost());
}


void MusicLEDMode::onUpdate(const RequestWrapper& request) {
    if (request.hasParam("fps")) {
        int val = request.getParam("fps")->value().toInt();
        setFPS(val);
        baseFPS = val;
    }
    if (request.hasParam("ledSpeed")) {
        int val = request.getParam("ledSpeed")->value().toInt();
        speed = val;
    }
    if (request.hasParam("hop")) {
        int val = request.getParam("hop")->value().toInt();
        pixelColorHop = val;
    }
    if (request.hasParam("reverse")) {
        int val = request.getParam("reverse")->value().toInt();
        reverse = val;
    }
    if (request.hasParam("bounce")) {
        int val = request.getParam("bounce")->value().toInt();
        bounce = val;
    }
    if (request.hasParam("decayfactor")) {
        int val = request.getParam("decayfactor")->value().toInt();
        decayFactor = val;
    }
    if (request.hasParam("beat")) {
        float beatPower = request.getParam("beat")->value().toFloat();
        beat(beatPower);
    }
}


float MusicLEDMode::getSpeedBoost() {
    return stabilizeSpeed();
}

unsigned long lastBeatTime = 0;

void MusicLEDMode::beat(float beatPower) {
    speedBoost = static_cast<float> (bounce) * beatPower; // Set the initial speed boost
    lastBeatTime = millis(); // Record the current time as the last beat time
}

float MusicLEDMode::stabilizeSpeed() {
    unsigned long currentTime = millis(); // Get the current time
    float timeSinceLastBeat = (currentTime - lastBeatTime) / 1000.0; // Convert to seconds

    // Apply exponential decay to the speed boost
    float decayFactorModifier = decayFactor / 100.0f;
    logger.debug(std::to_string(decayFactorModifier));
    speedBoost *= exp(-decayFactorModifier * timeSinceLastBeat);

    // Ensure speed boost doesn't go below zero
    speedBoost = max(speedBoost, 0.0f);

    return speedBoost;
}


void MusicLEDMode::incrementHue() {
    //Decreasing hue will lead to effect coming from "source" outwards
    currentHue -= !reverse ? speed : -speed;
}

void MusicLEDMode::debugButtonClick() {
    beat(1);
    logger.debug("Incoming beatttt!");
}

/**
 * Format: FPS, SPEED, PIXEL_HOP, REVERSE
 */
String MusicLEDMode::getSettings() {
    return String(baseFPS) + "," + String(speed) + "," + String(pixelColorHop) + "," + reverse + "," + String(bounce) +
           "," + String(decayFactor);
}
