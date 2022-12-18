#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include "Effects/BPMEffect.h"
#include "Utilities/ColorUtils.h"

#define TEST_BUTTON_PIN D6
#define LED_DATA_PIN D5
#define PIXEL_COUNT 6

#define DEFAULT_LED_FPS 400
#define TEST_BUTTON_FPS 20

const unsigned long ULONG_MAX = 0UL - 1UL;

void loop_LED(unsigned long timeNow);
void loop_testButton(unsigned long timeNow);
void readTestButton();
void testButtonClick();
void setFPS();


Adafruit_NeoPixel strip(PIXEL_COUNT, LED_DATA_PIN, NEO_BRG + NEO_KHZ400);
BPMEffect effect(&strip, PIXEL_COUNT, &setFPS);

int ACTIVE_LED_FPS = 1;

//Experiment with new STRIP now that its referenced and dereferenced through pointer. The LED Fade might work as of now, test

void setup() {
    Serial.begin(115200);
    strip.begin();

    pinMode(TEST_BUTTON_PIN, INPUT);
}


void loop() {
    unsigned long timeNow = micros();
    loop_LED(timeNow);
    loop_testButton(timeNow);
}

//The loop function for LED
unsigned long lastRun_FPS;
void loop_LED(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_FPS) % ULONG_MAX;
    if (timeLapsed > (1000000.0 / ACTIVE_LED_FPS)) {
        lastRun_FPS = timeNow;
        effect.loop();
    }
}

//The loop function for the test button
unsigned long lastRun_testButton;
void loop_testButton(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_testButton) % ULONG_MAX;
    if (timeLapsed > (1000000 / TEST_BUTTON_FPS)) {
        lastRun_testButton = timeNow;
        readTestButton();
    }
}


int lastInput;
void readTestButton() {
    int buttonInput = digitalRead(TEST_BUTTON_PIN);
    if (buttonInput == lastInput)
        return;
    lastInput = buttonInput;
    if (buttonInput == HIGH)
        testButtonClick();
}

void testButtonClick() {
    Serial.println("bob100");
    effect.testButtonClick();
}

void setFPS(int newFPS) {
    ACTIVE_LED_FPS = newFPS;
}