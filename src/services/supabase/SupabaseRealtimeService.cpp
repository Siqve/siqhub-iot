#include "SupabaseRealtimeService.h"
#include "services/supabase/utils/SupabaseUtils.h"
#include "services/supabase/utils/SupabaseRealtimeUtils.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"
#include "constants/SupabaseRealtimeConstants.h"
#include "utils/TimeUtils.h"

#ifdef ESP32

#include <HTTPClient.h>  // Include the WiFi library for ESP32

#elif defined(ESP8266)
#include <ESP8266HTTPClient.h>  // Include the WiFi library for ESP8266
#else
#error "Unsupported board! Please select an ESP32 or ESP8266 board."
#endif

using namespace SupabaseRealtimeUtils;
using namespace SupabaseRealtimeConstants::Response;

void SupabaseRealtimeService::loop() {
    if (!realtimeWebSocket.isConnected() && !realtimeConnecting) {
        connectRealtime();
        return;
    }
    manageRealtimeHeartbeat();
    realtimeWebSocket.loop();
}


void SupabaseRealtimeService::connectRealtime() {
    logger.info("Connecting to Supabase realtime websocket");

    const std::string hostname = SupabaseUtils::getHostname(SUPABASE_PROJECT_REFERENCE);
    const std::string realtimeSlug = getSlug(SUPABASE_API_KEY);
    realtimeWebSocket.beginSSL(hostname.c_str(), 443, realtimeSlug.c_str());

    realtimeWebSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
        this->onRealtimeEvent(type, payload, length);
    });
    realtimeConnecting = true;
}


void SupabaseRealtimeService::processRealtimeMessage(const std::string& message) {
    logger.debug("Processing realtime message: " + message);
    JsonDocument doc;
    deserializeJson(doc, message);

    const std::string& topic = doc[TOPIC_KEY].as<std::string>();
    const std::string topicFiltered = getTopicFiltered(topic);
    logger.info("Received message on topic: " + topicFiltered);
    if (!realtimeChannelCallbacks.contains(topicFiltered)) {
        logger.error("No callback found.");
        return;
    }
    if (!doc[PAYLOAD_KEY][PAYLOAD_DATA_KEY].is<JsonVariant>()) {
        logger.info("No data found in payload. Ignoring message.");
        return;
    }

    logger.info("Calling callback for topic: " + topicFiltered);
    const JsonVariantConst payloadData = doc[PAYLOAD_KEY][PAYLOAD_DATA_KEY];
    realtimeChannelCallbacks[topicFiltered](payloadData);
}

void SupabaseRealtimeService::onRealtimeEvent(const WStype_t type, uint8_t* payload, const size_t length) {
    switch (type) {
        case WStype_TEXT:
            processRealtimeMessage(std::string((char*) payload, length));
            break;
        case WStype_DISCONNECTED:
            logger.info("Realtime websocket disconnected");
            break;
        case WStype_CONNECTED:
            logger.info("Realtime websocket connected");
            realtimeConnecting = false;
            break;
        case WStype_ERROR:
            logger.error("Realtime websocket error. Payload: " + std::string((char*) payload, length));
            break;
        default:
            logger.info("Received unsupported realtime websocket event: " + std::to_string(type));
            break;
    }
}


bool
SupabaseRealtimeService::addUpdateListener(const std::string& topic, const std::string& table, const std::string& filter,
                                           const std::function<void(const JsonVariantConst&)>& callback) {
    if (!realtimeWebSocket.isConnected()) {
        return false;
    }
    logger.info("Creating update listener for table: " + table + ", with filter: " + filter + ", and topic: " + topic);
    realtimeWebSocket.sendTXT(createUpdateConnectionString(topic, table, filter).c_str());
    realtimeChannelCallbacks[topic] = [callback](const JsonVariantConst& data) {
        JsonVariantConst recordProperty = data[UPDATE_RECORD_KEY];
        callback(recordProperty);
    };
    return true;
}

bool
SupabaseRealtimeService::addInsertListener(const std::string& topic, const std::string& table, const std::function<void(const JsonVariantConst&)>& callback) {
    if (!realtimeWebSocket.isConnected()) {
        return false;
    }
    logger.info("Creating insert listener for table: " + table + ", with topic: " + topic);
    realtimeWebSocket.sendTXT(createInsertConnectionString(topic, table).c_str());
    realtimeChannelCallbacks[topic] = [callback](const JsonVariantConst& data) {
        JsonVariantConst recordProperty = data[UPDATE_RECORD_KEY];
        callback(recordProperty);
    };
    return true;
}


static uint32_t lastHeartbeatMillis = 0;

void SupabaseRealtimeService::manageRealtimeHeartbeat() {
    if (!TimeUtils::isMillisElapsed(millis(), lastHeartbeatMillis, 30000)) {
        return;
    }
    lastHeartbeatMillis = millis();
    logger.info("Sending Supabase Realtime heartbeat");
    realtimeWebSocket.sendTXT(createHeartbeat().c_str());
}

