#include "ArduinoJson.h"
#include "utils/TextUtils.h"

namespace SupabaseFilterUtils {

    std::string equals(const std::string& column, const std::string& value) {
        return column + "=eq." + value;
    }

}