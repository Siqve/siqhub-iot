#ifndef ARDUINO_CONSTANTS_H
#define ARDUINO_CONSTANTS_H

#include "Arduino.h"

extern const unsigned long ULONG_MAXVAL;

namespace LEDConstants {
    extern const int LED_DATA_PIN;
}

namespace WiFiConstants {
    extern const char* WIFI_SSID;
    extern const char* WIFI_PW;
}

#endif //ARDUINO_CONSTANTS_H
