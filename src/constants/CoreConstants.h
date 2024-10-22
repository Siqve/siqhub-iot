#pragma once
#include <string>
#include <unordered_map>

namespace CoreConstants {

    class DeviceType {
    public:
        enum class Value {
            LED_STRIP,
            UNKNOWN
        };

        static const std::unordered_map<std::string, Value> stringToEnum;
        static const std::unordered_map<Value, std::string> enumToString;

        static Value from(const std::string &value);
        static std::string toString(Value value);

    };


};

