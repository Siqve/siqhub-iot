#include "SupabaseService.h"
#include "utils/SupabaseUtils.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"
#include "constants/SupabaseConstants.h"

#ifdef ESP32

#include <HTTPClient.h>  // Include the WiFi library for ESP32

#elif defined(ESP8266)
#include <ESP8266HTTPClient.h>  // Include the WiFi library for ESP8266
#else
#error "Unsupported board! Please select an ESP32 or ESP8266 board."
#endif


// INSPO https://github.com/jhagas/ESPSupabase/blob/master/src/Realtime.cpp

void SupabaseService::loop() {
    if (!realtimeWebSocket.isConnected() && !realtimeConnecting) {
        connectRealtime();
        return;
    }
    manageRealtimeHeartbeat();
    realtimeWebSocket.loop();
}


void SupabaseService::connectRealtime() {
    logger.info("Connecting to Supabase realtime websocket");

    std::string hostname = SupabaseUtils::getHostname(SUPABASE_PROJECT_REFERENCE);
    std::string realtimeSlug = SupabaseUtils::Realtime::getSlug(SUPABASE_API_KEY);
    realtimeWebSocket.beginSSL(hostname.c_str(), 443, realtimeSlug.c_str());
    realtimeWebSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
        this->onRealtimeEvent(type, payload, length);
    });
    realtimeConnecting = true;
}


void SupabaseService::processRealtimeMessage(const std::string& message) {
    logger.debug("Processing realtime message: " + message);
    JsonDocument doc;
    deserializeJson(doc, message);

    auto topic = doc[SupabaseConstants::Realtime::TOPIC_KEY].as<std::string>();
    logger.info(std::string("Received message on topic: ") + topic);

    auto topicFiltered = SupabaseUtils::Realtime::getTopicFiltered(topic);
    if (realtimeChannelCallbacks.find(topicFiltered) == realtimeChannelCallbacks.end()) {
        logger.error("No callback found for topic: " + topicFiltered);
        return;
    }
    logger.info("Calling callback for topic: " + topicFiltered);
    if (doc[SupabaseConstants::Realtime::PAYLOAD_KEY].containsKey(SupabaseConstants::Realtime::PAYLOAD_DATA_KEY)) {
        auto payloadData = doc[SupabaseConstants::Realtime::PAYLOAD_KEY][SupabaseConstants::Realtime::PAYLOAD_DATA_KEY];
        realtimeChannelCallbacks[topicFiltered](payloadData);
    }
}

void SupabaseService::onRealtimeEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            logger.info("Realtime websocket disconnected");
            break;
        case WStype_CONNECTED:
            logger.info("Realtime websocket connected");
            realtimeConnecting = false;
            break;
        case WStype_TEXT:
            processRealtimeMessage(std::string((char*) payload, length));
            break;
        case WStype_ERROR:
            logger.error("Realtime websocket error. Payload: " + std::string((char*) payload, length));
            break;
        default:
            logger.info("Received unsupported realtime websocket event: " + std::to_string(type));
            break;
    }
}


/**
 * Create a realtime channel for the given table, filter, and topic.
 * The callback will be called when a message is received on the channel.
 * @return true if the channel was created successfully, false otherwise
 */
bool
SupabaseService::createRealtimeChannel(const std::string& table, const std::string& filter, const std::string& topic,
                                       const std::function<void(const JsonVariantConst&)>& callback) {
    if (!realtimeWebSocket.isConnected()) {
        return false;
    }
    logger.info("Creating realtime channel for table: " + table + ", with filter: " + filter + ", and topic: " + topic);
    realtimeWebSocket.sendTXT(SupabaseUtils::Realtime::createJoinMessage(table, filter, topic).c_str());
    realtimeChannelCallbacks[topic] = callback;
    return true;
}

std::optional<JsonDocument> SupabaseService::select(const std::string& table, const std::string& column, const std::string& value) {
    return sendRestRequest(table + "?" + SupabaseUtils::Filters::equals(column, value));
}

std::optional<JsonDocument> SupabaseService::sendRestRequest(const std::string& slug, const std::string& body) {
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient httpsClient;
    httpsClient.setTimeout(10000);

    std::string restUrl = SupabaseUtils::getRestApiUrl(SUPABASE_PROJECT_REFERENCE, slug);
    logger.info("Sending request to: " + restUrl);

    if (!httpsClient.begin(wifiClient, restUrl.c_str())) {
        logger.error("Unable to connect to request URL");
        return std::nullopt;
    }

    httpsClient.addHeader("apiKey", SUPABASE_API_KEY);
    int httpCode;
    if (body.empty()) {
        httpCode = httpsClient.GET();
    } else {
        httpsClient.addHeader("Content-Type", "application/json");
        httpCode = httpsClient.POST(body.c_str());
    }

    if (httpCode <= 0) {
        logger.error(("Request failed. Error: " + HTTPClient::errorToString(httpCode)).c_str());
        httpsClient.end();
        return std::nullopt;
    }

    std::string responseString = httpsClient.getString().c_str();
    logger.debug("Response: " + responseString);

    JsonDocument doc;
    deserializeJson(doc, responseString);
    httpsClient.end();
    return doc;
}


uint32_t lastHeartbeatMillis = 0;

void SupabaseService::manageRealtimeHeartbeat() {
    unsigned long timeNow = millis();
    if (timeNow - lastHeartbeatMillis < 30000)
        return;
    logger.info("Sending Supabase Realtime heartbeat");
    realtimeWebSocket.sendTXT(SupabaseUtils::Realtime::createHeartbeat().c_str());
    lastHeartbeatMillis = timeNow;

}
