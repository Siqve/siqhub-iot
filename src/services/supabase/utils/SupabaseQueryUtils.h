#pragma once

#include <string>
#include <optional>

namespace SupabaseQueryUtils {

    std::string getRestApiUrl(const std::string& reference, const std::string& table, const std::string& selectQuery = "");

    namespace Filters {
        std::string equals(const std::string& column, const std::string& value);
    }

}
