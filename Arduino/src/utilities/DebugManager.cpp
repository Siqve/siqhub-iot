#include "DebugManager.h"


void DebugManager::onDebug(const std::string& command) {
    std::string log = "[DebugManager] Command received: " + command;
    loggerObject.debug(command);
}

DebugManager::Logger& DebugManager::logger() {
    return loggerObject;
}

void DebugManager::Logger::log(const std::string& line, const std::string& logType) {
    logBuffer << "[" + logType + "] " << line << "\n";
    logUpdateId++;
    Serial.print("[");
    Serial.print(logType.c_str());
    Serial.print("] ");
    Serial.println(line.c_str());
}


std::string DebugManager::Logger::getLogFeed() {
    return logBuffer.str();
}

void DebugManager::Logger::clearLog() {
    logBuffer.str("");
    logBuffer.clear();
}

int DebugManager::Logger::getLogUpdateId() {
    return logUpdateId;
}

void DebugManager::Logger::info(const std::string& line) {
    log(line, "info");
}

void DebugManager::Logger::warn(const std::string& line) {
    log(line, "warn");
}

void DebugManager::Logger::error(const std::string& line) {
    log(line, "error");
}

void DebugManager::Logger::debug(const std::string& line) {
    log(line, "debug");
}
