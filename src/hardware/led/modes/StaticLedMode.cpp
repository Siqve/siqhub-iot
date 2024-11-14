#include <constants/LedConstants.h>

#include "StaticLedMode.h"

#include "utils/ColorUtils.h"
#include "hardware/led/utils/LedUtils.h"
#include "utils/TextUtils.h"

constexpr int FIXED_FPS = 1;

using namespace ColorUtils;

StaticLedMode::StaticLedMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> &ledStrip,
                             std::function<void(int)> setFps) : LedMode(ledStrip, Debug::Logger("StaticLedMode"), std::move(setFps)) {
}


void StaticLedMode::handleUpdate(const JsonDocument &settings) {
    setFps(FIXED_FPS);

    const std::string colorString = settings[LedConstants::Settings::Static::Mode::COLOR_KEY];
    staticColor = Gamma32(hexStringToColor(colorString));
    loop();
}

void StaticLedMode::loop() {
    logger.info("Looping StaticLedMode");
    LedUtils::setSolidColor(ledStrip, staticColor);
    ledStrip.Show();
}

void StaticLedMode::onDebugCommand(const std::string &command) {
    logger.debug("Incoming debug command: " + command);
    std::istringstream commandParser(command);
    std::string firstArgument = TextUtils::parseNextWord(commandParser);
    if (firstArgument.empty())
        return;
    bool isInt = std::find_if(firstArgument.begin(),
                              firstArgument.end(), [](unsigned char c) { return !std::isdigit(c); }) == firstArgument.end();

    if (isInt) {
        int pixelIndex = std::stoi(firstArgument);
        LedUtils::setSolidColor(ledStrip, 0);
        ledStrip.SetPixelColor(pixelIndex, ColorUtils::colorToRgbColor(16711680));
        ledStrip.Show();
    }
}
