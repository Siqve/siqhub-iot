#include "LEDController.h"
#include "Constants.h"
#include "LED/modes/FadeLEDMode.h"
#include "LED/modes/StaticLEDMode.h"
#include "LED/modes/MusicLEDMode.h"

void LEDController::initEffects() {
    modes.push_back(std::make_shared<StaticLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<FadeLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<MusicLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
}

void LEDController::setup() {
    initEffects();
    LEDStripPtr.Begin();
    getActiveMode()->onActivate();
    Debug::DebugCommandHandler::getInstance().registerListener("led", [this](std::string& command) {
        getActiveMode()->onDebugCommand(command);
    });
}

void LEDController::loop() {
    unsigned long timeNow = micros();
    loop_LED(timeNow);
}

//The loop function for LED
unsigned long lastRun_FPS;

void LEDController::loop_LED(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_FPS) % ULONG_MAXVAL;
    if (timeLapsed > (1000000.0 / activeFPS)) {
        lastRun_FPS = timeNow;
        getActiveMode()->loop();
    }
}

void LEDController::debugButtonClick() {
    getActiveMode()->debugButtonClick();
}

void LEDController::setFPS(int newFPS) {
    activeFPS = newFPS;
}

void LEDController::incomingUpdate(AsyncWebServerRequest* request) {
    if (request->hasParam("mode")) {
        int newMode = request->getParam("mode")->value().toInt();
        if (newMode < 0 || newMode >= static_cast<int>(modes.size())) {
            logger.warn(std::string("Received request for nonexistent mode: ") + std::to_string(newMode));
            return;
        }
        activeModeNumber = newMode;
        logger.info(std::string("Active mode updated to: ") + std::to_string(activeModeNumber));
        getActiveMode()->onActivate();
        return;
    }
    getActiveMode()->onUpdate(request);
}


void LEDController::incomingDebug() {
    debugButtonClick();
}

int LEDController::getActiveModeNumber() {
    return activeModeNumber;
}

std::shared_ptr<LEDMode> LEDController::getActiveMode() {
    return modes[activeModeNumber];
}

String LEDController::getModeAndSettings() {
    return String(activeModeNumber) + "," + getActiveMode()->getSettings();
}
