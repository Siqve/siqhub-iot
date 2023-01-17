#include "StaticLEDMode.h"

#include "utilities/ColorUtils.h"
#include "utilities/StripUtils.h"

StaticLEDMode::StaticLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr, int pixelCount,
                             const std::function<void(int)>& setFPS) : LEDMode(std::move(LEDStripPtr), pixelCount) {
    this->LEDStripPixelCount = pixelCount;
    setFPS(1);
}

void StaticLEDMode::onActivate() {
    StripUtils::setSolidColor(LEDStripPtr, LEDStripPixelCount, defaultColor);
    LEDStripPtr->show();
}


void StaticLEDMode::loop() {
    LEDStripPtr->show();
}

void StaticLEDMode::onUpdate(AsyncWebServerRequest *request) {
    if (request->hasParam("staticcolor")) {
        String val = request->getParam("staticcolor")->value();
        uint32_t color = ColorUtils::hexStringToColor(val.c_str());
        StripUtils::setSolidColor(LEDStripPtr, LEDStripPixelCount, color);
        LEDStripPtr->show();
    }
}