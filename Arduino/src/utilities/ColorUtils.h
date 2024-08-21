#pragma once

#include "NeoPixelBus.h"

namespace ColorUtils {
    extern uint32_t hexStringToColor(const char* hexString);
    extern uint32_t HSVToColor(uint16_t hue, uint8_t sat, uint8_t val);
    extern RgbColor HSVToRgbColor(uint16_t hue, uint8_t sat, uint8_t val);
    extern uint32_t Gamma32(uint32_t x);
    extern uint8_t Gamma8(uint8_t x);
    extern uint32_t rgbToColor(uint8_t r, uint8_t g, uint8_t b);
    extern int * colorToRGB(uint32_t color);
    extern RgbColor colorToRgbColor(uint32_t color);
}