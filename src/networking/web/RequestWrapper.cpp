#include <LittleFS.h>
#include "RequestWrapper.h"

static uint16_t HTTP_CODE_OK = 200;
static uint16_t HTTP_CODE_NOT_FOUND = 404;

bool RequestWrapper::hasParam(const std::string& paramName) const {
    return request->hasParam(paramName.c_str());
}

AsyncWebParameter* RequestWrapper::getParam(const std::string& paramName) const {
    return request->getParam(paramName.c_str());
}

AsyncWebServerResponse* RequestWrapper::ok(const std::string& response) const {
    return createResponsePlain(HTTP_CODE_OK, response);
}

AsyncWebServerResponse* RequestWrapper::okJson(const std::string& response) const {
    return createResponseJson(HTTP_CODE_OK, response);
}

AsyncWebServerResponse* RequestWrapper::notFound(const std::string& response) const {
    return createResponsePlain(HTTP_CODE_NOT_FOUND, response);
}

AsyncWebServerResponse* RequestWrapper::createResponsePlain(int status, const std::string& responseContent) const {
    AsyncWebServerResponse* response = request->beginResponse(status, "text/plain; charset=utf-8", responseContent.c_str());
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    return response;
}

std::function<void(AsyncWebServerRequest*)>
RequestWrapper::useWrapper(const std::function<AsyncWebServerResponse*(const RequestWrapper&)>& callback) {
    return [callback](AsyncWebServerRequest* originalRequest) {
        RequestWrapper requestWrapper(originalRequest);
        AsyncWebServerResponse* response = callback(requestWrapper);
        originalRequest->send(response);
    };
}
AsyncWebServerResponse* RequestWrapper::createResponseJson(int status, const std::string& responseContent) const {
    String finalResponse = String("{")
                           + "\"status\": " + status;
    if (!responseContent.empty()) {
        finalResponse += ", \"response\": " + String(responseContent.c_str());
    }
    finalResponse += "}";

    AsyncWebServerResponse* response = request->beginResponse(status, "text/json; charset=utf-8", finalResponse);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    return response;
}