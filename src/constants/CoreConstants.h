#pragma once
#include <string>

namespace CoreConstants {

    class DeviceType {
    public:
        enum class Value {
            LED_STRIP,
            UNKNOWN
        };

        static Value from(const std::string &value);

    };


};

