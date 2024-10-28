#include "SupabaseUtils.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"

namespace SupabaseUtils {

    std::string getHostname(const std::string& reference) {
        return reference + ".supabase.co";
    }

    std::string getUrl(const std::string& reference) {
        return "https://" + getHostname(reference);
    }

    std::string getRestApiUrl(const std::string& reference, const std::string& table, const std::string& selectQuery) {
        return SupabaseUtils::getUrl(reference) + "/rest/v1/" + table;
    }

}