#pragma once

#include "WebSocketsClient.h"
#include "debug/Logger.h"
#include <cstdint>
#include "ArduinoJson.h"
#include <optional>
#include <map>

class SupabaseService {
public:
    SupabaseService(const SupabaseService&) = delete;
    SupabaseService& operator=(const SupabaseService&) = delete;

    static SupabaseService& getInstance() {
        static SupabaseService instance;
        return instance;
    }

    void loop();

    [[nodiscard]] bool addRealtimeListener(const std::string& table, const std::string& filter, const std::string& topic, const std::function<void(const JsonVariantConst&)>& callback);
    std::optional<JsonDocument> select(const std::string& table, const std::string& column, const std::string& value);

private:
    SupabaseService() = default;

    // Realtime
    void connectRealtime();
    void onRealtimeEvent(WStype_t type, uint8_t* payload, size_t length);
    void processRealtimeMessage(const std::string& message);
    void manageRealtimeHeartbeat();

    // Rest
    std::optional<JsonDocument> sendRestRequest(const std::string& slug, const std::string& body = "");

    WebSocketsClient realtimeWebSocket;
    std::map<std::string, std::function<void(const JsonVariantConst&)>> realtimeChannelCallbacks;

    bool realtimeConnecting = false;
    Debug::Logger logger = Debug::Logger("SupabaseService");

};

