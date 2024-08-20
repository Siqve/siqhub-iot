#ifndef ARDUINO_REQUESTWRAPPER_H
#define ARDUINO_REQUESTWRAPPER_H


#include "ESPAsyncWebServer.h"

class RequestWrapper {
public:
    explicit RequestWrapper(AsyncWebServerRequest* request) : request(request) {}

    static std::function<void(AsyncWebServerRequest*)>
    useWrapper(const std::function<AsyncWebServerResponse*(const RequestWrapper&)>& originalRequest);

    // Params
    bool hasParam(const std::string& paramName) const;
    AsyncWebParameter* getParam(const std::string& paramName) const;

    // Responses
    AsyncWebServerResponse* okJson(const std::string& responseJson = "") const;
    AsyncWebServerResponse* ok(const std::string& responsePlain = "") const;
    AsyncWebServerResponse* notFound(const std::string& responseJson = "") const;


private:
    AsyncWebServerRequest* request;
    AsyncWebServerResponse* createResponse(int status, const std::string& responseContent) const;

};


#endif //ARDUINO_REQUESTWRAPPER_H
