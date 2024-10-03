#pragma once

#include <string>
#include <Optional>

namespace SupabaseUtils {

    std::string getHostname(const std::string& reference);
    std::string getUrl(const std::string& reference);
    std::string getRestApiUrl(const std::string& reference, const std::string& table, const std::string& selectQuery = "");

    namespace Filters {
        std::string getApiUrl(const std::string& reference, const std::string& slug);
        std::string equals(const std::string& column, const std::string& value);

    }

    namespace Realtime {
        std::string getSlug(const std::string& apiKey);
        std::string createJoinMessage(const std::string& table, const std::string& filter, const std::optional<std::string>& = std::nullopt);
        std::string createLeaveMessage(const std::string& table, const std::string& filter);
        std::string createHeartbeat();
        std::string getTopicFiltered(const std::string& topic);
    }

}