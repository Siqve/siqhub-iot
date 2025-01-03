#pragma once

#include "WebSocketsClient.h"
#include "debug/Logger.h"
#include <cstdint>
#include "ArduinoJson.h"
#include <optional>
#include <map>

class SupabaseQueryService {
public:
    SupabaseQueryService(const SupabaseQueryService &) = delete;
    SupabaseQueryService &operator=(const SupabaseQueryService &) = delete;

    static SupabaseQueryService &getInstance() {
        static SupabaseQueryService instance;
        return instance;
    }

    std::optional<JsonDocument> select(const std::string &table, const std::string &column, const std::string &value);

private:
    SupabaseQueryService() = default;

    std::optional<JsonDocument> sendRestRequest(const std::string &slug, const std::string &body = "");

    Debug::Logger logger = Debug::Logger("SupabaseQueryService");
};

