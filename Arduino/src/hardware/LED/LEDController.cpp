#include "LEDController.h"
#include "hardware/LED/modes/FadeLEDMode.h"
#include "hardware/LED/modes/StaticLEDMode.h"
#include "hardware/LED/modes/MusicLEDMode.h"
#include "networking/web/WebServer.h"
#include "debug/DebugCommandHandler.h"

void LEDController::initEffects() {
    modes.push_back(std::make_shared<StaticLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<FadeLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<MusicLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
}

void LEDController::setup() {
    initEffects();
    LEDStripPtr.Begin();
    getActiveMode()->onActivate();

    WebServer::getInstance().registerPageCallback("/update", [this](const RequestWrapper& request) {
        return onUpdate(request);
    });

//        TODO, respond with a correct JSON format
//    WebServerManager::getInstance().registerPageCallback("/led", [this](const RequestWrapper& request) {
//        request.ok(getModeAndSettings().c_str());
//    });

    Debug::DebugCommandHandler::getInstance().registerListener("led", [this](std::string& command) {
        getActiveMode()->onDebugCommand(command);
    });
}

void LEDController::loop() {
    unsigned long timeNow = micros();
    loop_LED(timeNow);
}

//The loop function for LED
unsigned long lastLoopTime;
void LEDController::loop_LED(unsigned long timeNow) {
    if (timeNow - lastLoopTime < (1000000.0 / activeFPS)) {
        return;
    }
    lastLoopTime = timeNow;
    getActiveMode()->loop();
}

void LEDController::debugButtonClick() {
    getActiveMode()->debugButtonClick();
}

void LEDController::setFPS(int newFPS) {
    activeFPS = newFPS;
}

AsyncWebServerResponse* LEDController::onUpdate(const RequestWrapper& request) {
    if (request.hasParam("mode")) {
        int newMode = request.getParam("mode")->value().toInt();
        if (newMode < 0 || newMode >= static_cast<int>(modes.size())) {
            logger.warn(std::string("Received request for nonexistent mode: ") + std::to_string(newMode));
            return request.ok();
        }
        activeModeNumber = newMode;
        logger.info(std::string("Active mode updated to: ") + std::to_string(activeModeNumber));
        getActiveMode()->onActivate();
        return request.ok();
    }
    getActiveMode()->onUpdate(request);
    return request.ok();
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
