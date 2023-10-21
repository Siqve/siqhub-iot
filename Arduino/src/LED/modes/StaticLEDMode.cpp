#include "StaticLEDMode.h"

#include "utilities/ColorUtils.h"
#include "utilities/StripUtils.h"

StaticLEDMode::StaticLEDMode(std::shared_ptr<Adafruit_NeoPixel> LEDStripPtr,
                             std::function<void(int)> setFPS) : LEDMode(std::move(LEDStripPtr), std::move(setFPS)) {
}

void StaticLEDMode::onActivate() {
    setFPS(1);
    StripUtils::setSolidColor(LEDStripPtr, defaultColor);
    LEDStripPtr->show();
}


void StaticLEDMode::loop() {
    LEDStripPtr->show();
}

void StaticLEDMode::onUpdate(AsyncWebServerRequest *request) {
    if (request->hasParam("staticcolor")) {
        String val = request->getParam("staticcolor")->value();
        uint32_t color = ColorUtils::hexStringToColor(val.c_str());
        uint32_t gammaCorrected = ColorUtils::Gamma32(color);
        StripUtils::setSolidColor(LEDStripPtr, gammaCorrected);
        LEDStripPtr->show();
    }
}