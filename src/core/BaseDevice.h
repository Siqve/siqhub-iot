#pragma once

#include <utility>

#include "ArduinoJson.h"

class BaseDevice {
public:
    virtual ~BaseDevice() = default;
    virtual void loop() = 0;
    virtual void handleSettingsUpdate(const JsonDocument &settings) = 0;
    virtual void initialize(const JsonDocument &settings) = 0;
    virtual int getFps() = 0;

    [[nodiscard]] std::string getType() const {
        return type;
    }

protected:
    explicit BaseDevice(std::string type) : type(std::move(type)) {}

    explicit BaseDevice() = default;
    std::string type;
};