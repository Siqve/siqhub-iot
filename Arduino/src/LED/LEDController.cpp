#include "LEDController.h"
#include "Constants.h"
#include "LED/modes/FadeLEDMode.h"
#include "LED/modes/StaticLEDMode.h"
#include "LED/modes/MusicLEDMode.h"

#define TEST_BUTTON_PIN D6
#define DEFAULT_PIXEL_COUNT 47

#define DEFAULT_LED_FPS 400
#define TEST_BUTTON_FPS 20

LEDController::LEDController() : activeFPS(DEFAULT_LED_FPS) {
}

void LEDController::initEffects() {
    modes.push_back(std::make_shared<StaticLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<FadeLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<MusicLEDMode>(LEDStripPtr, [this](int newFPS) { setFPS(newFPS); }));
}

void LEDController::setup(int ledDataPin) {
    LEDStripPtr = std::make_shared<Adafruit_NeoPixel>(DEFAULT_PIXEL_COUNT, ledDataPin, NEO_BRG + NEO_KHZ400);
    initEffects();
    LEDStripPtr->begin();
    getActiveMode()->onActivate();
//    pinMode(TEST_BUTTON_PIN, INPUT);
}

void LEDController::loop() {
    unsigned long timeNow = micros();
    loop_LED(timeNow);
//    loop_physicalDebugButton(timeNow);
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

//The loop function for the test button
unsigned long lastRun_testButton;

void LEDController::loop_physicalDebugButton(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_testButton) % ULONG_MAXVAL;
    if (timeLapsed > (1000000 / TEST_BUTTON_FPS)) {
        lastRun_testButton = timeNow;
        readPhysicalDebugButton();
    }
}

int lastInput;

void LEDController::readPhysicalDebugButton() {
    int buttonInput = digitalRead(TEST_BUTTON_PIN);
    if (buttonInput == lastInput)
        return;
    lastInput = buttonInput;
    if (buttonInput == HIGH)
        debugButtonClick();
}

void LEDController::debugButtonClick() {
    getActiveMode()->debugButtonClick();
}

void LEDController::setFPS(int newFPS) {
    Serial.print("Updating FPS! ");
    Serial.println(newFPS);
    activeFPS = newFPS;
}

void LEDController::incomingUpdate(AsyncWebServerRequest *request) {
    if (request->hasParam("mode")) {
        int newMode = request->getParam("mode")->value().toInt();
        if (newMode < 0 || newMode >= static_cast<int>(modes.size())) {
            Serial.println("[LEDController] Received request for nonexistent activeMode: ");
            Serial.println(newMode);
            return;
        }
        activeModeNumber = newMode;
        Serial.print("[LEDController] New Active Mode: ");
        Serial.println(activeModeNumber);
        getActiveMode()->onActivate();
        return;
    }
    if (request->hasParam("pixelcount")) {
        int pixelCount = request->getParam("pixelcount")->value().toInt();
        LEDStripPtr->updateLength(pixelCount);
        return;
    }
    getActiveMode()->onUpdate(request);
}


void LEDController::incomingDebug(AsyncWebServerRequest *request) {
    if (request->hasParam("buttonClick")) {
        debugButtonClick();
    }
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
