#include "StaticLedMode.h"

#include "utils/ColorUtils.h"
#include "utils/ledUtils.h"
#include "utils/TextUtils.h"

const char* REQUEST_PARAM_STATIC_COLOR = "static-color";

StaticLedMode::StaticLedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& ledStrip,
                             std::function<void(int)> setFps) : LedMode(ledStrip, Debug::Logger("StaticLedMode"), std::move(setFps)) {
}

void StaticLedMode::initialize(const JsonDocument& settings) {
    setFps(0);
    if (settings.containsKey("activeColorId")) {
        staticColor = settings["activeColorId"];
    }
    ledUtils::setSolidColor(ledStrip, staticColor);
    ledStrip.Show();
}


void StaticLedMode::loop() {
    ledUtils::setSolidColor(ledStrip, staticColor);
    ledStrip.Show();
}

void StaticLedMode::onUpdate(const RequestWrapper& request) {
    if (request.hasParam(REQUEST_PARAM_STATIC_COLOR)) {
        String val = request.getParam(REQUEST_PARAM_STATIC_COLOR)->value();
        uint32_t color = ColorUtils::hexStringToColor(val.c_str());
        uint32_t gammaCorrected = ColorUtils::Gamma32(color);
        staticColor = gammaCorrected;
        loop();
    }
}

void StaticLedMode::onDebugCommand(const std::string& command) {
    logger.debug("Incoming debug command: " + command);
    std::istringstream commandParser(command);
    std::string firstArgument = TextUtils::parseNextWord(commandParser);
    if (firstArgument.empty())
        return;
    bool isInt = std::find_if(firstArgument.begin(),
                              firstArgument.end(), [](unsigned char c) { return !std::isdigit(c); }) == firstArgument.end();

    if (isInt) {
        int pixelIndex = std::stoi(firstArgument);
        ledUtils::setSolidColor(ledStrip, 0);
        ledStrip.SetPixelColor(pixelIndex, ColorUtils::colorToRgbColor(16711680));
        ledStrip.Show();
        return;
    }
}
