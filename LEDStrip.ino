#include <Adafruit_NeoPixel.h>
#include "BPMEffect.h"
#include "ColorUtils.h"

#define TEST_BUTTON_PIN 11
#define LED_DATA_PIN 3
#define PIXEL_COUNT 6

#define DEFAULT_LED_FPS 400
#define TEST_BUTTON_FPS 50

const unsigned long ULONG_MAX = 0UL - 1UL;

Adafruit_NeoPixel strip(PIXEL_COUNT, LED_DATA_PIN, NEO_BRG + NEO_KHZ400);
BPMEffect effect(strip, PIXEL_COUNT, &setFPS);

float LED_FPS = 1;

void setup() {
  Serial.begin(9600);
  strip.begin();
  Serial.println("New Compile");
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
  if (timeLapsed > (1000000 / LED_FPS)) {
      lastRun_FPS = timeNow;
      //manageTestButton();  
      effect.loop();
  }
}

//The loop function for the test button
unsigned long lastRun_testButton;
void loop_testButton(unsigned long timeNow) {
  unsigned long timeLapsed = (timeNow - lastRun_testButton) % ULONG_MAX;
  if (timeLapsed > (1000000 / TEST_BUTTON_FPS)) {
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
  effect.testButtonClick();
}

void setFPS(int newFPS) {
  LED_FPS = newFPS;
}
