#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include "Effects/BPMEffect.h"
#include "web/WiFiSession.h"
#include "web/ControllerWebServer.h"
#include "Constants.h"

#define TEST_BUTTON_PIN D6
#define LED_DATA_PIN D5
#define PIXEL_COUNT 6

#define DEFAULT_LED_FPS 400
#define TEST_BUTTON_FPS 20

void loop_LED(unsigned long timeNow);
void loop_testButton(unsigned long timeNow);
void readTestButton();
void testButtonClick();
void setFPS(int fps);

WiFiSession wifiSession(WiFiConstants::WIFI_SSID, WiFiConstants::WIFI_PW);
ControllerWebServer webServer = ControllerWebServer();

Adafruit_NeoPixel strip(PIXEL_COUNT, LED_DATA_PIN, NEO_BRG + NEO_KHZ400);
BPMEffect effect(&strip, PIXEL_COUNT, &setFPS);

int ACTIVE_LED_FPS = DEFAULT_LED_FPS;

void setup() {
    Serial.begin(115200);
    strip.begin();
    wifiSession.startSession();
    webServer.initServer();

    pinMode(TEST_BUTTON_PIN, INPUT);
}



void loop() {
    wifiSession.assureConnection();
    unsigned long timeNow = micros();
    loop_LED(timeNow);
    loop_testButton(timeNow);
}

//The loop function for LED
unsigned long lastRun_FPS;
void loop_LED(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_FPS) % ULONG_MAXVAL;
    if (timeLapsed > (1000000.0 / ACTIVE_LED_FPS)) {
        lastRun_FPS = timeNow;
        effect.loop();
    }
}

//The loop function for the test button
unsigned long lastRun_testButton;
void loop_testButton(unsigned long timeNow) {
    unsigned long timeLapsed = (timeNow - lastRun_testButton) % ULONG_MAXVAL;
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