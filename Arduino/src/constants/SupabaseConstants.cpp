#include "SupabaseConstants.h"

namespace SupabaseConstants {

    namespace Realtime {
        const std::string TOPIC_KEY = "topic";
        const std::string PAYLOAD_KEY = "payload";
        const std::string PAYLOAD_DATA_KEY = "data";
        const std::string UPDATE_RECORD_KEY = "record";
    }

    namespace Tables {

        namespace Device {

            const std::string TABLE_NAME = "device";
            const std::string COLUMN_ID = "id";
            const std::string COLUMN_TYPE = "type";
            const std::string COLUMN_SETTINGS_JSON = "settings_json";

        }

    }

}