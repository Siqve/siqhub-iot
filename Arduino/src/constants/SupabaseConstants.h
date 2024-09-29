#pragma once

#include <string>

namespace SupabaseConstants {

    namespace Realtime {
        extern const std::string TOPIC_KEY;
        extern const std::string PAYLOAD_KEY;
        extern const std::string PAYLOAD_DATA_KEY;
        extern const std::string UPDATE_RECORD_KEY;
    }

    namespace Tables {

        namespace Device {

            extern const std::string TABLE_NAME;
            extern const std::string COLUMN_ID;
            extern const std::string COLUMN_TYPE;
            extern const std::string COLUMN_SETTINGS_JSON;

        }

    }
}
