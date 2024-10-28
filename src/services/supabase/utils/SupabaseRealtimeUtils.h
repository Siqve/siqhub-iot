#pragma once

#include <string>
#include <optional>

namespace SupabaseRealtimeUtils {

    std::string getSlug(const std::string& apiKey);
    std::string createJoinMessage(const std::string& table, const std::string& filter, const std::optional<std::string>& = std::nullopt);
    std::string createLeaveMessage(const std::string& table, const std::string& filter);
    std::string createHeartbeat();
    std::string getTopicFiltered(const std::string& topic);

}
