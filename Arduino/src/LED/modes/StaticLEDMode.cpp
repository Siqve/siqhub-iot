#include "StaticLEDMode.h"

#include "utilities/ColorUtils.h"
#include "utilities/StripUtils.h"

const char* REQUEST_PARAM_STATIC_COLOR = "static-color";

StaticLEDMode::StaticLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr,
                             std::function<void(int)> setFPS) : LEDMode(LEDStripPtr, std::move(setFPS)) {
}

void StaticLEDMode::onActivate() {
    setFPS(1);
    StripUtils::setSolidColor(LEDStripPtr, staticColor);
    LEDStripPtr.Show();
}


void StaticLEDMode::loop() {
    StripUtils::setSolidColor(LEDStripPtr, staticColor);
    LEDStripPtr.Show();
}

void StaticLEDMode::onUpdate(AsyncWebServerRequest *request) {
    if (request->hasParam(REQUEST_PARAM_STATIC_COLOR)) {
        String val = request->getParam(REQUEST_PARAM_STATIC_COLOR)->value();
        uint32_t color = ColorUtils::hexStringToColor(val.c_str());
        uint32_t gammaCorrected = ColorUtils::Gamma32(color);
        staticColor = gammaCorrected;
        loop();
    }
}