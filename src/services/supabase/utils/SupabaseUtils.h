#pragma once

#include <string>
#include <optional>

namespace SupabaseUtils {

    std::string getHostname(const std::string& reference);
    std::string getUrl(const std::string& reference);
    std::string getRestApiUrl(const std::string& reference, const std::string& table, const std::string& selectQuery = "");

}
