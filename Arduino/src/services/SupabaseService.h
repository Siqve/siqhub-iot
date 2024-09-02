#pragma once

#include "WebSocketsClient.h"
#include "debug/Logger.h"
#include <cstdint>
#include "ArduinoJson.h"

struct SupabaseToken {
    std::string accessToken;
    uint32_t expiresAt;
};

class SupabaseService {
public:
    SupabaseService() : logger(Debug::Logger("SupabaseClient")) {};

    void loop();
private:
    void initialize();

    void connectRealtime();
    void onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    void processWebSocketMessage(const std::string& message);
    void manageHeartbeat();

    bool checkToken();
    void acquireToken();

    std::unique_ptr<JsonDocument> sendRequest(const std::string& url, const std::string& query = "");

    WebSocketsClient webSocket;
    Debug::Logger logger;
    SupabaseToken token;

    bool initialized = false;
    bool connecting = false;

};

