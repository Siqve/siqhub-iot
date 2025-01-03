#pragma once

#include "ESPAsyncWebServer.h"

class RequestWrapper {
public:
    explicit RequestWrapper(AsyncWebServerRequest *request) : request(request) {}

    [[nodiscard]] static std::function<void(AsyncWebServerRequest *)>
    useWrapper(const std::function<AsyncWebServerResponse *(const RequestWrapper &)> &originalRequest);

    // Params
    [[nodiscard]] bool hasParam(const std::string &paramName) const;
    [[nodiscard]] AsyncWebParameter *getParam(const std::string &paramName) const;

    // Responses
    [[nodiscard]] AsyncWebServerResponse *okJson(const std::string &response = "") const;
    [[nodiscard]] AsyncWebServerResponse *ok(const std::string &response = "") const;
    [[nodiscard]] AsyncWebServerResponse *notFound(const std::string &response = "") const;


private:
    AsyncWebServerRequest *request;

    [[nodiscard]] AsyncWebServerResponse *createResponsePlain(int status, const std::string &responseContent) const;
    [[nodiscard]] AsyncWebServerResponse *createResponseJson(int status, const std::string &responseContent) const;
};