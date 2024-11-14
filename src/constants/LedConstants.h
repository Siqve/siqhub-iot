#pragma once

#include <string>
#include <unordered_map>

namespace LedConstants {
    namespace Settings {
        extern const std::string MODE_KEY;

        namespace Static {
            extern const std::string ACTIVE_COLOR_ID_KEY;

            namespace Mode {
                extern const std::string COLOR_KEY;
            }
        }
    }

    class LedModeType {
    public:
        enum class Value {
            STATIC,
            FADE,
            UNKNOWN
        };

        static Value from(const std::string &value);
        static std::string toString(Value value);

        private:
            static const std::unordered_map<std::string, Value> stringToEnum;
            static const std::unordered_map<Value, std::string> enumToString;
    };
}
