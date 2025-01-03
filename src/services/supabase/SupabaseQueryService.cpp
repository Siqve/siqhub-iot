#include "SupabaseQueryService.h"
#include "ArduinoJson.h"
#include "utils/TextUtils.h"
#include "services/supabase/utils/SupabaseFilterUtils.h"
#include "services/supabase/utils/SupabaseUrlUtils.h"

#ifdef ESP32

#include <HTTPClient.h>  // Include the WiFi library for ESP32

#elif defined(ESP8266)
#include <ESP8266HTTPClient.h>  // Include the WiFi library for ESP8266
#else
#error "Unsupported board! Please select an ESP32 or ESP8266 board."
#endif

using namespace SupabaseUrlUtils;

std::optional<JsonDocument> SupabaseQueryService::select(const std::string &table, const std::string &column, const std::string &value) {
    return sendRestRequest(table + "?" + SupabaseFilterUtils::equals(column, value));
}

std::optional<JsonDocument> SupabaseQueryService::sendRestRequest(const std::string &slug, const std::string &body) {
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient httpsClient;
    httpsClient.setTimeout(10000);

    std::string restUrl = getRestApiUrl(SUPABASE_PROJECT_REFERENCE, slug);
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