#pragma once


#include <string>

namespace LedConstants {

    namespace Settings {

        extern const std::string MODE_KEY;

        namespace Static {
            extern const std::string ACTIVE_COLOR_ID_KEY;
        }

    }

    class LedModeType {
    public:
        enum class Value {
            STATIC,
            FADE,
            UNKNOWN
        };

        static Value from(const std::string& value);

    };

}