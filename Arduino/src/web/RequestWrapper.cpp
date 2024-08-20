#include "RequestWrapper.h"

#define HTTP_CODE_OK 200
#define HTTP_CODE_NOT_FOUND 404

bool RequestWrapper::hasParam(const std::string& paramName) const {
    return request->hasParam(paramName.c_str());
}

AsyncWebParameter* RequestWrapper::getParam(const std::string& paramName) const {
    return request->getParam(paramName.c_str());
}


AsyncWebServerResponse* RequestWrapper::ok(const std::string& responsePlain) const {
    return createResponse(HTTP_CODE_OK, responsePlain.empty() ? "" : "\"" + responsePlain + "\"");
}

AsyncWebServerResponse* RequestWrapper::okJson(const std::string& responseJson) const {
    return createResponse(HTTP_CODE_OK, responseJson);
}

AsyncWebServerResponse* RequestWrapper::notFound(const std::string& responseJson) const {
    return createResponse(HTTP_CODE_NOT_FOUND, responseJson);
}


AsyncWebServerResponse* RequestWrapper::createResponse(int status, const std::string& responseContent) const {
    String finalResponse = String("{")
                           + "\"status\": " + status;
    if (!responseContent.empty()) {
        finalResponse += ", \"response\": " + String(responseContent.c_str());
    }
    finalResponse += "}";

    AsyncWebServerResponse* response = request->beginResponse(status, "text/json", finalResponse);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
    return response;
}


std::function<void(AsyncWebServerRequest*)>
RequestWrapper::useWrapper(const std::function<AsyncWebServerResponse*(const RequestWrapper&)>& callback) {
    return [callback](AsyncWebServerRequest* originalRequest) {
        RequestWrapper requestWrapper(originalRequest);
        auto response = callback(requestWrapper);
        originalRequest->send(response);
    };
}

