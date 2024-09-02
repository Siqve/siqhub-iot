#include "SupabaseUtils.h"
#include "ArduinoJson.h"

namespace SupabaseUtils {

    std::string getHostname(const std::string& reference) {
        return reference + ".supabase.co";
    }

    std::string getUrl(const std::string& reference) {
        return "https://" + reference + ".supabase.co/";
    }

    std::string getTokenUrl(const std::string& reference) {
        return getUrl(reference) + "auth/v1/token?grant_type=password";
    }

    std::string getRESTUrl(const std::string& reference, const std::string& table, const std::string& selectQuery) {
        return getUrl(reference) + "rest/v1/" + table;
    }

    namespace Realtime {

        std::string getSlug(const std::string& apiKey) {
            return "/realtime/v1/websocket?apikey=" + apiKey + "&log_level=info&vsn=1.0.0";
        }

        std::string createJoinMessage(const std::string& table, const std::string& filter) {
            JsonDocument json;
            json["event"] = "phx_join";
            json["topic"] = std::string("realtime:") + PIOENV + ":" + table + ":" + filter;
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
    }


}
