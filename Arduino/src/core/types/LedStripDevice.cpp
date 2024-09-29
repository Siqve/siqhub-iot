#include "LedStripDevice.h"
#include "utils/TimeUtils.h"

LedStripDevice::LedStripDevice() : BaseDevice("LED_STRIP") {
}

//void LedStripDevice::initEffects() {
//    modes.push_back(std::make_shared<StaticLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
//    modes.push_back(std::make_shared<FadeLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
//    modes.push_back(std::make_shared<MusicLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
//}


void LedStripDevice::applySettings(const JsonDocument& settings) {

}

static uint32_t lastLoopTimeMillis;
void LedStripDevice::loop() {
    if (!TimeUtils::isMillisElapsed(millis(), lastLoopTimeMillis, 2000)) {
        return;
    }
    lastLoopTimeMillis = millis();
    logger.info("LedStripDevice loop");
}

