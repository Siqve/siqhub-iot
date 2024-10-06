#include "CoreConstants.h"

#include <unordered_map>


namespace CoreConstants {

    // Static function to convert string to enum
    DeviceType::Value DeviceType::from(const std::string &value) {
        static const std::unordered_map<std::string, Value> stringToEnum = {
            {"LED_STRIP", Value::LED_STRIP},
        };

        auto iterator = stringToEnum.find(value);
        if (iterator != stringToEnum.end()) {
            return iterator->second;
        }
        return Value::UNKNOWN; // Fallback in case the string is invalid
    }

}
