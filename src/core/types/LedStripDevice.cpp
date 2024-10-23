#include "LedStripDevice.h"

#include "utils/TimeUtils.h"
#include "hardware/led/modes/StaticLedMode.h"
#include "hardware/led/modes/FadeLedMode.h"
#include "constants/LedConstants.h"

using namespace LedConstants;

LedStripDevice::LedStripDevice() : BaseDevice("LED_STRIP") {
    initEffects();
    ledStrip.Begin();
}


void LedStripDevice::initialize(const JsonDocument &settings) {
    logger.info("Initializing LedStripDevice");
    updateSettings(settings);
}


static uint32_t lastLoopTimeMillis;

void LedStripDevice::loop() {
    if (!TimeUtils::isMillisElapsed(millis(), lastLoopTimeMillis, 5000)) {
        return;
    }
    lastLoopTimeMillis = millis();
    logger.info("LedStripDevice loop");
    getMode()->loop();
}

void LedStripDevice::updateSettings(const JsonDocument &settings) {
    logger.info("Updating settings for LedStripDevice");
    const std::string &modeSetting = settings[Settings::MODE_KEY].as<std::string>();
    const LedModeType::Value ledModeType = LedModeType::from(modeSetting);

    if (ledModeType == LedModeType::Value::UNKNOWN) {
        activeModeType = LedModeType::Value::STATIC;
        logger.error("Unknown LED mode set: " + modeSetting + ". Defaulting to STATIC.");
    } else {
        activeModeType = ledModeType;
    }

    if (activeModeType == LedModeType::Value::STATIC) {
        // TODO: Register color listener for active color id
    }

    getMode()->initialize(settings);
}


void LedStripDevice::initEffects() {
    ledModes[LedModeType::Value::STATIC] = std::make_shared<StaticLedMode>(ledStrip, [this](int newFps) { setFPS(newFps); });
    ledModes[LedModeType::Value::FADE] = std::make_shared<FadeLedMode>(ledStrip, [this](int newFps) { setFPS(newFps); });
}

std::shared_ptr<LedMode> LedStripDevice::getMode() {
    return ledModes[activeModeType];
}

void LedStripDevice::setFPS(int newFPS) {
    activeFps = newFPS;
}