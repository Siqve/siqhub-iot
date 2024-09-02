#include "WebServer.h"
#include "utils/TextUtils.h"

void WebServer::init() {
    server.begin();
    logger.info("HTTP server started.");
    onLandingPage();
    onLogger();
}


// Just used to indicate that server is running
void WebServer::onLandingPage() {
    registerPageCallback("/", [](const RequestWrapper& request) {
        return request.ok();
    });
}

// Request to retrieve logger information
void WebServer::onLogger() {
    registerPageCallback("/logger", [](const RequestWrapper& request) {
        if (request.hasParam("update-id")) {
            return request.ok(std::to_string(Debug::Logger::getLogUpdateId()));
        }
        return request.ok(TextUtils::replaceAll(Debug::Logger::getLogFeed(), "\n", "<br>"));
    });
}

void WebServer::registerPageCallback(const std::string& path, const std::function<AsyncWebServerResponse*(
        const RequestWrapper&)>& callback) {
    logger.info("Registering callback for path: " + path);
    server.on(path.c_str(), HTTP_GET, RequestWrapper::useWrapper(callback));
}
