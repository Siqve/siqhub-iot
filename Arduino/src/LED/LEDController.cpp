#include "Arduino.h"
#include "LEDController.h"
#include "Constants.h"
#include "LED/modes/FadeLEDMode.h"
#include "LED/modes/StaticLEDMode.h"
#include "LED/modes/MusicLEDMode.h"

#define TEST_BUTTON_PIN D6
#define LED_DATA_PIN D5
#define PIXEL_COUNT 6

#define DEFAULT_LED_FPS 400
#define TEST_BUTTON_FPS 20


LEDController::LEDController() : activeFPS(DEFAULT_LED_FPS) {
}

void LEDController::initEffects() {
    modes.push_back(std::make_shared<StaticLEDMode>(LEDStripPtr, PIXEL_COUNT, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<FadeLEDMode>(LEDStripPtr, PIXEL_COUNT, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<MusicLEDMode>(LEDStripPtr, PIXEL_COUNT, [this](int newFPS) { setFPS(newFPS); }));
}

void LEDController::setup() {
    pinMode(TEST_BUTTON_PIN, INPUT);
    LEDStripPtr = std::make_shared<Adafruit_NeoPixel>(PIXEL_COUNT, LED_DATA_PIN, NEO_BRG + NEO_KHZ400);
    initEffects();
    LEDStripPtr->begin();
//TODO: Find out why this LEDStrip code works but not the one on onActivate on staticledmode, maybe different ledstip objects?
}

void LEDController::loop() {
    unsigned long timeNow = micros();
    loop_LED(timeNow);
    loop_physicalDebugButton(timeNow);
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
    activeFPS = newFPS;
}

void LEDController::incomingUpdate(AsyncWebServerRequest *request) {
    //TODO: Code for web debugbuttonclick, which calls LEDController::debugButtonClick
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
//    Serial.println("[LEDController] Request received.");
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
