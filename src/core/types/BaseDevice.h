#pragma once

#include <utility>

#include "ArduinoJson.h"

class BaseDevice {
public:
    virtual ~BaseDevice() = default;
    virtual void loop() = 0;
    virtual void updateSettings(const JsonDocument& settings) = 0;
    virtual void setup(const JsonDocument& settings) = 0;

    [[nodiscard]] std::string getType() const {
        return type;
    }

protected:
    explicit BaseDevice(std::string type) : type(std::move(type)) {}

    explicit BaseDevice() = default;
    std::string type;
};