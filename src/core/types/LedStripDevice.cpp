#include "LedStripDevice.h"

#include "utils/TimeUtils.h"
#include "hardware/LED/modes/StaticLEDMode.h"
#include "hardware/LED/modes/FadeLEDMode.h"
#include "constants/LedConstants.h"

using namespace LedConstants;

LedStripDevice::LedStripDevice() : BaseDevice("LED_STRIP") {
    initEffects();
    ledStrip.Begin();
}

void LedStripDevice::initialize(const JsonDocument &settings) {
    // TODO: Test this
    const std::string &modeSetting = settings[Settings::MODE_KEY].as<std::string>();
    const LedModeType::Value ledModeType = LedModeType::from(modeSetting);

    if (ledModeType == LedModeType::Value::UNKNOWN) {
        logger.error("Unknown LED mode set: " + modeSetting + ". Defaulting to STATIC.");
        activeModeType = LedModeType::Value::STATIC;
    } else {
        activeModeType = ledModeType;
    }

    if (activeModeType == LedModeType::Value::STATIC) {
        // TODO: Register color listener for active color id
    }

    getActiveMode()->initialize(settings);
}


static uint32_t lastLoopTimeMillis;

void LedStripDevice::loop() {
    if (!TimeUtils::isMillisElapsed(millis(), lastLoopTimeMillis, 2000)) {
        return;
    }
    lastLoopTimeMillis = millis();
    logger.info("LedStripDevice loop");
    getActiveMode()->loop();
}

void LedStripDevice::updateSettings(const JsonDocument &settings) {
    //TODO
}


void LedStripDevice::initEffects() {
    ledModes[LedModeType::Value::STATIC] = std::make_shared<StaticLEDMode>(ledStrip, [this](int newFps) { setFPS(newFps); });
    ledModes[LedModeType::Value::FADE] = std::make_shared<FadeLEDMode>(ledStrip, [this](int newFps) { setFPS(newFps); });
}

std::shared_ptr<LEDMode> LedStripDevice::getActiveMode() {
    return ledModes[activeModeType];
}

void LedStripDevice::setFPS(int newFPS) {
    activeFps = newFPS;
}
