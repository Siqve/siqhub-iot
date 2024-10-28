#include "SupabaseUtils.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"

namespace SupabaseQueryUtils {

    std::string getRestApiUrl(const std::string& reference, const std::string& table, const std::string& selectQuery) {
        return SupabaseUtils::getUrl(reference) + "/rest/v1/" + table;
    }

    namespace Filters {

        std::string equals(const std::string& column, const std::string& value) {
            return column + "=eq." + value;
        }

    }
}