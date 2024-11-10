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

    // Static function to convert string to enum
    LedModeType::Value LedModeType::from(const std::string &value) {
        static const std::unordered_map<std::string, Value> stringToEnum = {
            {"STATIC", Value::STATIC},
            {"FADE", Value::FADE},
        };

        auto iterator = stringToEnum.find(value);
        if (iterator != stringToEnum.end()) {
            return iterator->second;
        }
        return Value::UNKNOWN; // Fallback in case the string is invalid
    }
}
