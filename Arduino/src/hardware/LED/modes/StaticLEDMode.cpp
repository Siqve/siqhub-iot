#include "StaticLEDMode.h"

#include "utils/ColorUtils.h"
#include "utils/LEDUtils.h"
#include "utils/TextUtils.h"

const char* REQUEST_PARAM_STATIC_COLOR = "static-color";

StaticLEDMode::StaticLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr,
                             std::function<void(int)> setFPS) : LEDMode(LEDStripPtr, Debug::Logger("StaticLEDMode"), std::move(setFPS)) {
}

void StaticLEDMode::onActivate() {
    setFPS(0);
    LEDUtils::setSolidColor(LEDStripPtr, staticColor);
    LEDStripPtr.Show();
}


void StaticLEDMode::loop() {
    LEDUtils::setSolidColor(LEDStripPtr, staticColor);
    LEDStripPtr.Show();
}

void StaticLEDMode::onUpdate(const RequestWrapper& request) {
    if (request.hasParam(REQUEST_PARAM_STATIC_COLOR)) {
        String val = request.getParam(REQUEST_PARAM_STATIC_COLOR)->value();
        uint32_t color = ColorUtils::hexStringToColor(val.c_str());
        uint32_t gammaCorrected = ColorUtils::Gamma32(color);
        staticColor = gammaCorrected;
        loop();
    }
}

void StaticLEDMode::onDebugCommand(const std::string& command) {
    logger.debug("Incoming debug command: " + command);
    std::istringstream commandParser(command);
    std::string firstArgument = TextUtils::parseNextWord(commandParser);
    if (firstArgument.empty())
        return;
    bool isInt = std::find_if(firstArgument.begin(),
                              firstArgument.end(), [](unsigned char c) { return !std::isdigit(c); }) == firstArgument.end();

    if (isInt) {
        int pixelIndex = std::stoi(firstArgument);
        LEDUtils::setSolidColor(LEDStripPtr, 0);
        LEDStripPtr.SetPixelColor(pixelIndex, ColorUtils::colorToRgbColor(LEDConstants::DEFAULT_STATIC_COLOR));
        LEDStripPtr.Show();
        return;
    }
}
