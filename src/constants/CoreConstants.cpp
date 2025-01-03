#include "CoreConstants.h"

#include <unordered_map>


namespace CoreConstants {

    const std::unordered_map<std::string, DeviceType::Value> DeviceType::stringToEnum = {
            {"LED_STRIP", DeviceType::Value::LED_STRIP},
    };
    const std::unordered_map<DeviceType::Value, std::string> DeviceType::enumToString = {
            {DeviceType::Value::LED_STRIP, "LED_STRIP"},
            {DeviceType::Value::UNKNOWN,   "UNKNOWN"},
    };

    // Static function to convert string to enum
    DeviceType::Value DeviceType::from(const std::string &value) {
        auto iterator = stringToEnum.find(value);
        if (iterator != stringToEnum.end()) {
            return iterator->second;
        }
        return Value::UNKNOWN;
    }

    std::string DeviceType::toString(DeviceType::Value value) {
        auto iterator = enumToString.find(value);
        if (iterator != enumToString.end()) {
            return iterator->second;
        }
        return enumToString.at(Value::UNKNOWN);
    }
}
