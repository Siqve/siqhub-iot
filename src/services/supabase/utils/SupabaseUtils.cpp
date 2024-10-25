#include "SupabaseUtils.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"

namespace SupabaseUtils {

    std::string getHostname(const std::string& reference) {
        return reference + ".supabase.co";
    }

    std::string getUrl(const std::string& reference) {
        return "https://" + reference + ".supabase.co/";
    }

    std::string getRestApiUrl(const std::string& reference, const std::string& table, const std::string& selectQuery) {
        return getUrl(reference) + "rest/v1/" + table;
    }

    namespace Realtime {

        std::string getSlug(const std::string& apiKey) {
            return "/realtime/v1/websocket?apikey=" + apiKey + "&log_level=info&vsn=1.0.0";
        }

        std::string createJoinMessage(const std::string& table, const std::string& filter,
                                      const std::optional<std::string>& topic) {
            JsonDocument json;
            json["event"] = "phx_join";
            if (topic) {
                json["topic"] = std::string("realtime:") + topic.value();
            } else {
                json["topic"] = std::string("realtime:") + PIOENV + ":" + table + ":" + filter;
            }
            json["payload"]["config"]["postgres_changes"][0]["event"] = "UPDATE";
            json["payload"]["config"]["postgres_changes"][0]["schema"] = "public";
            json["payload"]["config"]["postgres_changes"][0]["table"] = table;
            json["payload"]["config"]["postgres_changes"][0]["filter"] = filter;
            json["ref"] = "Creating join message. Device: " + std::string(PIOENV) + ", table: " + table + ", filter: " +
                          filter;
            std::string message;
            serializeJson(json, message);
            return message;
        }

        std::string createLeaveMessage(const std::string& topic) {
            JsonDocument json;
            json["event"] = "phx_leave";
            json["topic"] = std::string("realtime:") + topic;
            json["payload"] = "{}";
            json["ref"] = "Leaving topic: " + topic;
            std::string message;
            serializeJson(json, message);
            return message;
        }

        std::string createHeartbeat() {
            JsonDocument json;
            json["event"] = "heartbeat";
            json["topic"] = "phoenix";
            json["payload"] = "{}";
            json["ref"] = "heartbeat";
            std::string message;
            serializeJson(json, message);
            return message;
        }

        std::string getTopicFiltered(const std::string& topic) {
            return TextUtils::replaceAll(topic, "realtime:", "");
        }
    }

    namespace Filters {

        std::string equals(const std::string& column, const std::string& value) {
            return column + "=eq." + value;
        }

    }
}