#include "DebugCommandHandler.h"

#include <utility>
#include "utilities/CommandUtils.h"

namespace Debug {

    bool DebugCommandHandler::execute(const std::string& command) {
        std::string log = "Debug command received: " + command;
        logger.debug(log);
        std::istringstream commandParser(command);

        std::string initialCommand = CommandUtils::parseNextWord(commandParser);

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

    void
    DebugCommandHandler::registerListener(const std::string& cmd, std::function<void(std::string&)> callback) {
        commandListeners[cmd] = std::move(callback);
    }

}
