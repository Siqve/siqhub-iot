#ifndef ARDUINO_MUSICLEDMODE_H
#define ARDUINO_MUSICLEDMODE_H


#include "LED/LEDMode.h"

class MusicLEDMode : public LEDMode {
public:
    MusicLEDMode(NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod>& LEDStripPtr, std::function<void(int)> setFPS);
    void onActivate() override;
    void loop() override;
    void onUpdate(const RequestWrapper& request) override;
    void debugButtonClick() override;
    String getSettings() override;
private:
    void cycleFade();
    void beat(float beatPower);
    void incrementHue();
    void updateFPS();
    float getSpeedBoost();
    float stabilizeSpeed();

    bool reverse = false;
    float speedBoost = 0;
    int baseFPS;
    int speed;
    int pixelColorHop;
    int bounce;
    int decayFactor = 40;
    uint16_t currentHue = 0;
};

#endif //ARDUINO_MUSICLEDMODE_H
