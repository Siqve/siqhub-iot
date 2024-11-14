#include "LedConstants.h"

#include <unordered_map>


namespace LedConstants {
    namespace Settings {
        const std::string MODE_KEY = "mode";

        namespace Static {
            const std::string ACTIVE_COLOR_ID_KEY = "activeColorId";

            namespace Mode {
                const std::string COLOR_KEY = "color";
            }
        }
    }

    const std::unordered_map<std::string, LedModeType::Value> LedModeType::stringToEnum = {
        {"STATIC", LedModeType::Value::STATIC},
        {"FADE", LedModeType::Value::FADE},
    };
    const std::unordered_map<LedModeType::Value, std::string> LedModeType::enumToString = {
        {LedModeType::Value::STATIC, "STATIC"},
        {LedModeType::Value::FADE, "FADE"},
        {LedModeType::Value::UNKNOWN, "UNKNOWN"},
    };

    // Static function to convert string to enum
    LedModeType::Value LedModeType::from(const std::string &value) {
        auto iterator = stringToEnum.find(value);
        if (iterator != stringToEnum.end()) {
            return iterator->second;
        }
        return Value::UNKNOWN; // Fallback in case the string is invalid
    }

    std::string LedModeType::toString(LedModeType::Value value) {
        auto iterator = enumToString.find(value);
        if (iterator != enumToString.end()) {
            return iterator->second;
        }
        return enumToString.at(Value::UNKNOWN);
    }
}
