#include "LedStripDevice.h"
#include "utils/TimeUtils.h"
#include "hardware/LED/modes/StaticLEDMode.h"
#include "hardware/LED/modes/FadeLEDMode.h"

LedStripDevice::LedStripDevice() : BaseDevice("LED_STRIP") {
    initEffects();
}

void LedStripDevice::setup(const JsonDocument& settings) {
    ledStrip.Begin();

    getActiveMode()->onActivate();
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

void LedStripDevice::updateSettings(const JsonDocument& settings) {
//TODO: Make sure that getActiveMode->loop actually calls
// and make settings update the activeLedMode etc.
}


void LedStripDevice::initEffects() {
    modes.push_back(std::make_shared<StaticLEDMode>(ledStrip, [this](int newFps) { setFPS(newFps); }));
    modes.push_back(std::make_shared<FadeLEDMode>(ledStrip, [this](int newFPS) { setFPS(newFPS); }));
}

std::shared_ptr<LEDMode> LedStripDevice::getActiveMode() {
    return modes[activeModeNumber];
}

void LedStripDevice::setFPS(int newFPS) {
    activeFps = newFPS;
}