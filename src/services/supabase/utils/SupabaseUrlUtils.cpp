#include "SupabaseUrlUtils.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"

namespace SupabaseUrlUtils {

    std::string getHostname(const std::string &reference) {
        return reference + ".supabase.co";
    }

    std::string getUrl(const std::string &reference) {
        return "https://" + getHostname(reference);
    }

    std::string getRestApiUrl(const std::string &reference, const std::string &table, const std::string &selectQuery) {
        return SupabaseUrlUtils::getUrl(reference) + "/rest/v1/" + table;
    }

}