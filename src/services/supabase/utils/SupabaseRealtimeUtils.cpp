#include "SupabaseUtils.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"
#include "../constants/SupabaseRealtimeConstants.h"


namespace SupabaseRealtimeUtils {

    using namespace SupabaseRealtimeConstants;
    using namespace SupabaseRealtimeConstants::Request;

    std::string getSlug(const std::string& apiKey) {
        return "/realtime/v1/websocket?apikey=" + apiKey + "&log_level=info&vsn=1.0.0";
    }

    std::string createJoinMessage(const std::string& eventType, const std::string& topic, const std::string& table,
                                  const std::optional<std::string>& filter = std::nullopt) {
        JsonDocument json;
        json["event"] = PROTOCOL_JOIN;
        json["topic"] = TOPIC_PREFIX + topic;
        json["payload"]["config"]["postgres_changes"][0]["event"] = eventType;
        json["payload"]["config"]["postgres_changes"][0]["table"] = table;
        if (filter) {
            json["payload"]["config"]["postgres_changes"][0]["filter"] = filter.value();
        }
        json["payload"]["config"]["postgres_changes"][0]["schema"] = "public";
        json["ref"] =
                "Creating join message. Device: " + std::string(PIOENV) + ", table: " + table + ", filter: " + (filter ? filter.value() : "") + ", event: " +
                eventType;
        std::string message;
        serializeJson(json, message);
        return message;
    }

    std::string createUpdateConnectionString(const std::string& topic, const std::string& table, const std::string& filter) {
        return createJoinMessage("UPDATE", topic, table, filter);
    }

    std::string createInsertConnectionString(const std::string& topic, const std::string& table, const std::string& filter) {
        return createJoinMessage("INSERT", topic, table);
    }

    std::string createLeaveMessage(const std::string& topic) {
        JsonDocument json;
        json["event"] = PROTOCOL_LEAVE;
        json["topic"] = TOPIC_PREFIX + topic;
        json["payload"] = "{}";
        json["ref"] = "Leaving topic: " + topic;
        std::string message;
        serializeJson(json, message);
        return message;
    }

    std::string createHeartbeat() {
        JsonDocument json;
        json["event"] = PROTOCOL_HEARTBEAT;
        json["topic"] = "phoenix";
        json["payload"] = "{}";
        json["ref"] = "heartbeat";
        std::string message;
        serializeJson(json, message);
        return message;
    }

    std::string getTopicFiltered(const std::string& topic) {
        return TextUtils::replaceAll(topic, TOPIC_PREFIX, "");
    }

}