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

}