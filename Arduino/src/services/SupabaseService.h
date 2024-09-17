#pragma once

#include "WebSocketsClient.h"
#include "debug/Logger.h"
#include <cstdint>
#include "ArduinoJson.h"
#include <optional>
#include <map>

struct SupabaseToken {
    std::string accessToken;
    uint32_t expiresAt;
};

class SupabaseService {
public:
    SupabaseService() : logger(Debug::Logger("SupabaseClient")) {};
    SupabaseService(const SupabaseService&) = delete;
    SupabaseService& operator=(const SupabaseService&) = delete;

    static SupabaseService& getInstance() {
        static SupabaseService instance;
        return instance;
    }

    void loop();
    void createRealtimeChannel(const std::string& table, const std::string& filter, const std::string& topic, const std::function<void(const JsonObject&)>& callback);

private:
    void connectRealtime();

    void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    void processWebSocketMessage(const std::string& message);

    std::optional<JsonDocument> sendRequest(const std::string& url, const std::string& query = "");

    void manageHeartbeat();
    bool checkToken();
    void acquireToken();

    WebSocketsClient webSocket;
    Debug::Logger logger;
    SupabaseToken token;
    std::map<std::string, std::function<void(const JsonObject&)>> channelCallbacks;


    bool connecting = false;

};

