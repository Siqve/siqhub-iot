#include "StaticLEDMode.h"
#include "utilities/ColorUtils.h"

StaticLEDMode::StaticLEDMode(Adafruit_NeoPixel *LEDStrip, int pixelCount, std::function<void(int)> setFPS) {
    this->LEDStrip = *LEDStrip;
    this->LEDStripPixelCount = pixelCount;
    setFPS(1);
}

void StaticLEDMode::loop() {

}

void StaticLEDMode::onUpdate(AsyncWebServerRequest *request) {
    if (request->hasParam("staticcolor")) {
        String val = request->getParam("staticcolor")->value();
        Serial.println(val);
    }
}
