#pragma once

#include <string>
#include <optional>

namespace SupabaseUtils {

    std::string getHostname(const std::string& reference);
    std::string getUrl(const std::string& reference);

}
