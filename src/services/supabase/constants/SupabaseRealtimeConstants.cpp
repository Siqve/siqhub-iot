#include "SupabaseRealtimeConstants.h"

namespace SupabaseRealtimeConstants {

    const std::string TOPIC_PREFIX = "realtime:";

    namespace Request {
        const std::string PROTOCOL_JOIN = "phx_join";
        const std::string PROTOCOL_LEAVE = "phx_leave";
        const std::string PROTOCOL_HEARTBEAT = "heartbeat";
    }

    namespace Response {
        const std::string TOPIC_KEY = "topic";
        const std::string PAYLOAD_KEY = "payload";
        const std::string PAYLOAD_DATA_KEY = "data";
        const std::string UPDATE_RECORD_KEY = "record";
    }

}