#include "LedStripDevice.h"

LedStripDevice::LedStripDevice() : BaseDevice("LED_STRIP") {
}

//void LedStripDevice::initEffects() {
//    modes.push_back(std::make_shared<StaticLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
//    modes.push_back(std::make_shared<FadeLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
//    modes.push_back(std::make_shared<MusicLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
//}


void LedStripDevice::applySettings(const JsonDocument& settings) {

}

unsigned long lastLoopTime2;
void LedStripDevice::loop() {
//    if (timeNow - lastLoopTime < (1000000.0 / activeFPS)) {
    if (micros() - lastLoopTime2 < (1000000.0 / 1)) {
        return;
    }
    lastLoopTime2 = micros();
    logger.info("LedStripDevice loop");
}

