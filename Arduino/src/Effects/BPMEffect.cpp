#include "Arduino.h"
#include "BPMEffect.h"
#include "Utilities/ColorUtils.h"

#define BRIGHTNESS 255   /* Control the brightness of your leds */
#define SATURATION 255   /* Control the saturation of your leds */
#define SPEED 25   /* Control the saturation of your leds */
#define LED_COLOR_HOP 4000 /* The amount of hue increase each LED has to the previous*/
#define BASE_FPS 2

 BPMEffect::BPMEffect(Adafruit_NeoPixel* strip, int pixel_count, void fpsFunction(int)) {
  STRIP = *strip;
  PIXEL_COUNT = pixel_count;
  setFPS = fpsFunction;
  setFPS(BASE_FPS);
}

float speedBoost = 0;
uint16_t hue = 0;

unsigned long lastBeat = millis();

void BPMEffect::loop() {
  cycleFade();
  updateFPS();
  STRIP.show();
}


void BPMEffect::cycleFade() {
  Serial.println("bob7");
  incrementHue(true);
  for (int i = 0; i < PIXEL_COUNT; i++) {
    uint32_t color = ColorUtils::HSVToColor(hue + i*LED_COLOR_HOP, SATURATION, BRIGHTNESS);
    STRIP.setPixelColor(i, color);
  }
}

void BPMEffect::updateFPS() {
  setFPS(BASE_FPS + getSpeedBoost());
}


void BPMEffect::beat() {
  speedBoost = 16000;
}


float BPMEffect::getSpeedBoost() {
  return stabilizeSpeed();
}

float BPMEffect::stabilizeSpeed() {
  return speedBoost = max(speedBoost - 0.000005*pow(speedBoost,2), 0.0);
}


void BPMEffect::incrementHue(bool reverse) {
  //Decreasing hue will lead to effect coming from "source" outwards
  hue -= !reverse ? SPEED : -SPEED;
}

void BPMEffect::testButtonClick() {
  Serial.println("Test");
  beat();
}
