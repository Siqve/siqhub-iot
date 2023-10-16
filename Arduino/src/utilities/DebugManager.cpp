#include "DebugManager.h"


void DebugManager::onDebug(const std::string &command) {
    std::string serialLog = "[DebugManager] Command received: " + command;
    Serial.println(serialLog.c_str());
    logLine(command);
}


void DebugManager::logLine(const std::string& line, const std::string& logType) {
    logBuffer << "[" + logType + "] " << line << "\n";
    logUpdateId++;
}

void DebugManager::logLine(const String &line, const String &logType) {

    logBuffer << "[" << logType.c_str() << "] " << line.c_str() << "\n";
    logUpdateId++;
}

std::string DebugManager::getLogFeed() {
    return logBuffer.str();
}

void DebugManager::clearLog() {
    logBuffer.str("");
    logBuffer.clear();
}

int DebugManager::getLogUpdateId() {
    return logUpdateId;
}
