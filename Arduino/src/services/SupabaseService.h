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

    void loop();
    void createRealtimeChannel(const std::string& table, const std::string& filter, const std::string& topic, const std::function<void(const JsonDocument&)>& callback);

private:
    void initialize();

    void connectRealtime();
    void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    void processWebSocketMessage(const std::string& message);
    void manageHeartbeat();

    bool checkToken();
    void acquireToken();

    std::optional<JsonDocument> sendRequest(const std::string& url, const std::string& query = "");

    WebSocketsClient webSocket;
    Debug::Logger logger;
    SupabaseToken token;
    std::map<std::string, std::function<void(const JsonDocument&)>> channelCallbacks;


    bool connecting = false;

};

