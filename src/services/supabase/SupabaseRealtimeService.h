#pragma once

#include "WebSocketsClient.h"
#include "debug/Logger.h"
#include <cstdint>
#include "ArduinoJson.h"
#include <optional>
#include <map>

class SupabaseRealtimeService {
public:
    SupabaseRealtimeService(const SupabaseRealtimeService&) = delete;
    SupabaseRealtimeService& operator=(const SupabaseRealtimeService&) = delete;

    static SupabaseRealtimeService& getInstance() {
        static SupabaseRealtimeService instance;
        return instance;
    }

    void loop();

    [[nodiscard]] bool addUpdateListener(const std::string& topic, const std::string& table, const std::string& filter,
                                         const std::function<void(const JsonVariantConst&)>& callback);
    [[nodiscard]] bool addInsertListener(const std::string& topic, const std::string& table, const std::function<void(const JsonVariantConst&)>& callback);

private:
    SupabaseRealtimeService() = default;

    // Realtime
    void connectRealtime();
    void onRealtimeEvent(WStype_t type, uint8_t* payload, size_t length);
    void processRealtimeMessage(const std::string& message);
    void manageRealtimeHeartbeat();

    WebSocketsClient realtimeWebSocket;
    std::map<std::string, std::function<void(const JsonVariantConst&)>> realtimeChannelCallbacks;

    bool realtimeConnecting = false;
    Debug::Logger logger = Debug::Logger("SupabaseRealtimeService");

};

