#include "DebugManager.h"

#include <utility>
#include "utilities/CommandUtils.h"


void DebugManager::onDebugCommand(const std::string& command) {
    std::string log = "Debug command received: " + command;
    logger.debug(log);
    std::istringstream commandParser(command);

    std::string initialCommand = CommandUtils::parseNextWord(commandParser);
//    commandParser >> initialCommand;

    if (debugCommandListeners.find(initialCommand) == debugCommandListeners.end())
        return;

    std::string argumentsString;
    if (commandParser >> std::ws)
        std::getline(commandParser, argumentsString);

    debugCommandListeners[initialCommand](argumentsString);
}

void DebugManager::registerDebugCommandListener(const std::string& cmd, std::function<void(std::string&)> callback) {
    debugCommandListeners[cmd] = std::move(callback);
}
