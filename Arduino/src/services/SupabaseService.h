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
    void createRealtimeChannel(const std::string& table, const std::string& filter, const std::string& topic, const std::function<void(const JsonObject&)>& callback);

private:
    SupabaseService() = default;

    void connectRealtime();

    void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    void processWebSocketMessage(const std::string& message);

    std::optional<JsonDocument> sendRequest(const std::string& url, const std::string& query = "");

    void manageHeartbeat();

    WebSocketsClient webSocket;
    std::map<std::string, std::function<void(const JsonObject&)>> channelCallbacks;

    Debug::Logger logger = Debug::Logger("SupabaseService");

    bool connecting = false;

};

