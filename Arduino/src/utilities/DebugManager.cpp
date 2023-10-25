#include "DebugManager.h"


void DebugManager::onDebug(const std::string& command) {
    std::string log = "Command received: " + command;
    logger.debug(log);
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


void DebugManager::Logger::log(const std::string& line, const std::string& logType) {
    if (!className.empty()) {
        debugManager.logBuffer << "[" + className + "]";
        Serial.print("[");
        Serial.print(className.c_str());
        Serial.print("]");
    }
    if (!logType.empty()) {
        debugManager.logBuffer << "[" + logType + "] ";
        Serial.print("[");
        Serial.print(logType.c_str());
        Serial.print("] ");
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
