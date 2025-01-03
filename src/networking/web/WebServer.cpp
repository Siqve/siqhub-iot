#include <LittleFS.h>
#include "WebServer.h"
#include "utils/TextUtils.h"

static const char *RESPONSE_TYPE_HTML = "text/html; charset=utf-8";
static const char *RESPONSE_TYPE_CSS = "text/css; charset=utf-8";
static const char *RESPONSE_TYPE_JS = "text/javascript; charset=utf-8";

void WebServer::start() {
    server.begin();
    logger.info("HTTP server started.");
    onLandingPage();
    onDebugPage();
    onLogger();
}


// Just used to indicate that server is running
void WebServer::onLandingPage() {
    registerPageCallback("/", [](const RequestWrapper &request) {
        return request.ok();
    });
}

// Debug interface
void WebServer::onDebugPage() {
    server.on("/debug", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/debug.html", RESPONSE_TYPE_HTML);
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/style.css", RESPONSE_TYPE_CSS);
    });
    server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/main.js", RESPONSE_TYPE_JS);
    });
}

// Request to retrieve logger information
void WebServer::onLogger() {
    registerPageCallback("/logger", [](const RequestWrapper &request) {
        if (request.hasParam("update-id")) {
            return request.ok(std::to_string(Debug::Logger::getLogUpdateId()));
        }
        return request.ok(Debug::Logger::getLogFeed());
    });
}

void WebServer::registerPageCallback(const std::string &path, const std::function<AsyncWebServerResponse *(
        const RequestWrapper &)> &callback) {
    logger.info("Registering callback for path: " + path);
    server.on(path.c_str(), HTTP_GET, RequestWrapper::useWrapper(callback));
}
