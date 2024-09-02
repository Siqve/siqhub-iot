#include "SupabaseService.h"
#include "utilities/SupabaseUtils.h"
#include "ArduinoJson.h"

#ifdef ESP32
  #include <HTTPClient.h>  // Include the WiFi library for ESP32
#elif defined(ESP8266)
  #include <ESP8266HTTPClient.h>  // Include the WiFi library for ESP8266
#else
  #error "Unsupported board! Please select an ESP32 or ESP8266 board."
#endif

static const std::string TOKEN_RESPONSE_ACCESS_TOKEN_KEY = "access_token";
static const std::string TOKEN_RESPONSE_ERROR_DESCRIPTION_KEY = "error_description";
static const std::string TOKEN_RESPONSE_EXPIRES_AT_KEY = "expires_at";

// INSPO https://github.com/jhagas/ESPSupabase/blob/master/src/Realtime.cpp

void SupabaseService::loop() {
    if (!webSocket.isConnected() && !connecting) {
        connectRealtime();
        return;
    }
    manageHeartbeat();

    webSocket.loop();
}


void SupabaseService::connectRealtime() {
    logger.info("Connecting to Supabase WebSocket");

    std::string hostname = SupabaseUtils::getHostname(SUPABASE_PROJECT_REFERENCE);
    std::string realtimeSlug = SupabaseUtils::Realtime::getSlug(SUPABASE_API_KEY);
    logger.debug("Hostname: " + hostname);
    logger.debug("Realtime to: " + realtimeSlug);
    webSocket.beginSSL(hostname.c_str(), 443, realtimeSlug.c_str());
    webSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
        this->onWebSocketEvent(type, payload, length);
    });
    connecting = true;
}

void SupabaseService::onWebSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_DISCONNECTED:
            logger.info("WebSocket disconnected");
            break;
        case WStype_CONNECTED:
            logger.info("WebSocket connected");
            webSocket.sendTXT(SupabaseUtils::Realtime::createJoinMessage("color", "id=eq.20").c_str());
            connecting = false;
            break;
        case WStype_TEXT:
            logger.debug("WebSocket message received: " + std::string((char*) payload, length));
            break;
        case WStype_ERROR:
            logger.error("WebSocket error. Payload: " + std::string((char*) payload, length));
            break;
        default:
            logger.info("Received unsupported WebSocket event: " + std::to_string(type));
            break;
    }
}


std::unique_ptr<JsonDocument> SupabaseService::sendRequest(const std::string& url, const std::string& query) {
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient httpsClient;
    httpsClient.setTimeout(10000);

    logger.info("Sending request to: " + url);
    if (!httpsClient.begin(wifiClient, url.c_str())) {
        logger.error("Unable to connect to the token url");
        return nullptr;
    }

    httpsClient.addHeader("apiKey", SUPABASE_API_KEY);
    httpsClient.addHeader("Content-Type", "application/json");
    int httpCode;
    if (query.empty()) {
        httpCode = httpsClient.GET();
    } else {
        httpCode = httpsClient.POST(query.c_str());
    }

    if (httpCode <= 0) {
        logger.error(("Request failed. Error: " + HTTPClient::errorToString(httpCode)).c_str());
        httpsClient.end();
        return nullptr;
    }

    auto doc = std::unique_ptr<JsonDocument>(new JsonDocument());
    deserializeJson(*doc, httpsClient.getString());
    httpsClient.end();
    return doc;
}



void SupabaseService::processWebSocketMessage(const std::string& message) {
    JsonDocument doc;
    deserializeJson(doc, message);
}

uint32_t lastHeartbeatMillis = 0;
void SupabaseService::manageHeartbeat() {
    if (millis() - lastHeartbeatMillis >= 30000) {
        logger.info("Sending Supabase Realtime heartbeat");
        webSocket.sendTXT(SupabaseUtils::Realtime::createHeartbeat().c_str());
        lastHeartbeatMillis = millis();
    }
}


bool SupabaseService::checkToken() {
    if (token.expiresAt * 1000 < millis()) {
        if (token.expiresAt == 0) {
            logger.info("No token available.");
        } else {
            logger.info("Token expired.");
        }
        return false;
    }
    return true;
}


void SupabaseService::acquireToken() {
    logger.info("Starting token acquiring process");

    std::string postQuery =
            std::string(R"({"email": ")") + SUPABASE_USER_EMAIL + R"(", "password": ")" + SUPABASE_USER_PASSWORD +
            "\"}";

    auto responseJson = sendRequest(SupabaseUtils::getTokenUrl(SUPABASE_PROJECT_REFERENCE), postQuery);
    if (!responseJson) {
        logger.error("Token request failed. No response received");
        return;
    }

    if (!responseJson->containsKey(TOKEN_RESPONSE_ACCESS_TOKEN_KEY)) {
        logger.error("Token request failed. Error: " +
                     (responseJson->containsKey(TOKEN_RESPONSE_ERROR_DESCRIPTION_KEY)
                      ? (*responseJson)[TOKEN_RESPONSE_ERROR_DESCRIPTION_KEY].as<std::string>()
                      : "Unknown error"));
        return;
    }

    SupabaseToken newToken;
    newToken.accessToken = (*responseJson)[TOKEN_RESPONSE_ACCESS_TOKEN_KEY].as<std::string>();
    newToken.expiresAt = (*responseJson)[TOKEN_RESPONSE_EXPIRES_AT_KEY].as<uint32_t>();
    token = newToken;
    logger.info("token acquired: " + newToken.accessToken);
    logger.info("Supabase token acquired successfully");
}


