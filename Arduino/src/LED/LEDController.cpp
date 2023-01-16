#include "Arduino.h"
#include "LEDController.h"
#include "Constants.h"
#include "LED/modes/PulsatingLEDMode.h"
#include "LED/modes/StaticLEDMode.h"

#define TEST_BUTTON_PIN D6
#define LED_DATA_PIN D5
#define PIXEL_COUNT 6

#define DEFAULT_LED_FPS 400
#define TEST_BUTTON_FPS 20


LEDController::LEDController() : activeFPS(DEFAULT_LED_FPS) {
    initEffects();
    LEDStrip = Adafruit_NeoPixel(PIXEL_COUNT, LED_DATA_PIN, NEO_BRG + NEO_KHZ400);
}

void LEDController::initEffects() {
    modes.push_back(std::make_shared<StaticLEDMode>(&LEDStrip, PIXEL_COUNT, [this](int newFPS) { setFPS(newFPS); }));
    modes.push_back(std::make_shared<PulsatingLEDMode>(&LEDStrip, PIXEL_COUNT, [this](int newFPS) { setFPS(newFPS); }));
//    modes[1] = &pulsatingMode;
}

void LEDController::setup() {
    pinMode(TEST_BUTTON_PIN, INPUT);
    LEDStrip.begin();
}

void LEDController::loop() {
    unsigned long timeNow = micros();
    loop_LED(timeNow);
    loop_testButton(timeNow);
}

//The loop function for LED
unsigned long lastRun_FPS;

void LEDController::loop_LED(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_FPS) % ULONG_MAXVAL;
    if (timeLapsed > (1000000.0 / activeFPS)) {
        lastRun_FPS = timeNow;
//        effect.loop();
    }
}

//The loop function for the test button
unsigned long lastRun_testButton;

void LEDController::loop_testButton(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_testButton) % ULONG_MAXVAL;
    if (timeLapsed > (1000000 / TEST_BUTTON_FPS)) {
        lastRun_testButton = timeNow;
        readTestButton();
    }
}

int lastInput;

void LEDController::readTestButton() {
    int buttonInput = digitalRead(TEST_BUTTON_PIN);
    if (buttonInput == lastInput)
        return;
    lastInput = buttonInput;
    if (buttonInput == HIGH)
        testButtonClick();
}

void LEDController::testButtonClick() {
    Serial.println("bob100");
//    effect.testButtonClick();
}

void LEDController::setFPS(int newFPS) {
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
        activeMode = newMode;
        Serial.print("[LEDController] New Active Mode: ");
        Serial.println(activeMode);
        Serial.print("Size: ");
        Serial.println(modes.size());
        return;
    }
    Serial.println("[LEDController] Request received.");
    modes[activeMode]->onUpdate(request);
}

int LEDController::getActiveMode() {
    return activeMode;
}
