#pragma once

#include <string>

namespace SupabaseConstants {

    namespace Token {
        extern const std::string RESPONSE_ACCESS_TOKEN_KEY;
        extern const std::string RESPONSE_ERROR_DESCRIPTION_KEY;
        extern const std::string RESPONSE_EXPIRES_AT_KEY;
    }

    namespace Realtime {
        extern const std::string TOPIC_KEY;
        extern const std::string PAYLOAD_KEY;
        extern const std::string PAYLOAD_DATA_KEY;
    }
}
