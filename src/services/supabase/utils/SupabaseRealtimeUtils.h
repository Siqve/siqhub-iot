#pragma once

#include <string>
#include <optional>

namespace SupabaseRealtimeUtils {

    std::string getSlug(const std::string& apiKey);
    std::string createUpdateConnectionString(const std::string& topic, const std::string& table, const std::string& filter);
    std::string createInsertConnectionString(const std::string& topic, const std::string& table);
    std::string createLeaveMessage(const std::string& table, const std::string& filter);
    std::string createHeartbeat();
    std::string getTopicFiltered(const std::string& topic);

}
