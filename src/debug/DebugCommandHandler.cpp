#include "DebugCommandHandler.h"

#include "networking/web/RequestWrapper.h"
#include "networking/web/WebServer.h"
#include "utils/TextUtils.h"

namespace Debug {

    void DebugCommandHandler::initialize() {
        WebServer::getInstance().registerPageCallback("/debug-functions", [this](const RequestWrapper &request) {
            String cmd = request.getParam("cmd")->value();
            if (strcmp(cmd.c_str(), "ping") == 0) {
                logger.info("pong");
                return request.ok("pong");
            }
            if (strcmp(cmd.c_str(), "restart") == 0) {
                logger.info("Restarting device");
                ESP.restart();
                return request.ok("Restarting device");
            }

            if (!execute(cmd.c_str())) {
                return request.notFound();
            }

            return request.ok();
        });
    }


    bool DebugCommandHandler::execute(const std::string &command) {
        std::string log = "Debug command received: " + command;
        logger.debug(log);
        std::istringstream commandParser(command);

        std::string initialCommand = TextUtils::parseNextWord(commandParser);

        if (commandListeners.find(initialCommand) == commandListeners.end()) {
            return false;
        }

        std::string argumentsString;
        if (commandParser >> std::ws) {
            std::getline(commandParser, argumentsString);
        }

        commandListeners[initialCommand](argumentsString);
        return true;
    }

    void DebugCommandHandler::registerListener(const std::string &cmd, std::function<void(std::string &)> callback) {
        commandListeners[cmd] = std::move(callback);
    }

    void DebugCommandHandler::removeListener(const std::string &cmd) {
        commandListeners.erase(cmd);
    }


}
