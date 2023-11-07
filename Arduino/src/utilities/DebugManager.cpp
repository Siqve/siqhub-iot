#include "DebugManager.h"

#include <utility>
#include "CommandUtils.h"



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

void DebugManager::logDebug(const std::string& line) {
    logBuffer << "[Debug] " << line << "\n";
    logUpdateId++;
    Serial.print("[Debug] ");
    Serial.println(line.c_str());
}

DebugManager::Logger DebugManager::newLogger(const std::string& className) {
    return Logger(*this, className);
}

void DebugManager::registerDebugCommandListener(const std::string& cmd, std::function<void(std::string&)> callback) {
    debugCommandListeners[cmd] = std::move(callback);
}


void DebugManager::Logger::log(const std::string& line, const std::string& logType) {
    bool prefixAdded = false;
    if (!className.empty()) {
        debugManager.logBuffer << "[" + className + "]";
        Serial.print("[");
        Serial.print(className.c_str());
        Serial.print("]");
         prefixAdded = true;
    }
    if (!logType.empty()) {
        debugManager.logBuffer << "[" + logType + "]";
        Serial.print("[");
        Serial.print(logType.c_str());
        Serial.print("]");
        prefixAdded = true;
    }

    if (prefixAdded) {
        debugManager.logBuffer << " - ";
        Serial.print(" - ");
    }

    debugManager.logBuffer << line << "\n";
    debugManager.logUpdateId++;

    Serial.println(line.c_str());
}


void DebugManager::Logger::info(const std::string& line) {
    log(line, "Info");
}

void DebugManager::Logger::warn(const std::string& line) {
    log(line, "Warn");
}

void DebugManager::Logger::error(const std::string& line) {
    log(line, "Error");
}

void DebugManager::Logger::debug(const std::string& line) {
    log(line, "Debug");
}


std::string DebugManager::Logger::getLogFeed() {
    return debugManager.logBuffer.str();
}

void DebugManager::Logger::clearLog() {
    debugManager.logBuffer.str("");
    debugManager.logBuffer.clear();
}

int DebugManager::Logger::getLogUpdateId() {
    return debugManager.logUpdateId;
}
