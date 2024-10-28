#pragma once

#include <string>

namespace SupabaseRealtimeConstants {

    extern const std::string TOPIC_PREFIX;

    namespace Request {
        extern const std::string PROTOCOL_JOIN;
        extern const std::string PROTOCOL_LEAVE;
        extern const std::string PROTOCOL_HEARTBEAT;
    }

    namespace Response {
        extern const std::string TOPIC_KEY;
        extern const std::string PAYLOAD_KEY;
        extern const std::string PAYLOAD_DATA_KEY;
        extern const std::string UPDATE_RECORD_KEY;
    }

}
