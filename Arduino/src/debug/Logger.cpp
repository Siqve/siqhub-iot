//
// Created by Siqve on 11/08/2024.
//

#include "Logger.h"

namespace Debug {

    std::ostringstream Logger::logBuffer;
    int Logger::logUpdateId = 0;
    int Logger::maxLogBufferSize = 2048;


    void Logger::info(const std::string& line) {
        log(line, "Info", className);
    }

    void Logger::warn(const std::string& line) {
        log(line, "Warn", className);
    }

    void Logger::error(const std::string& line) {
        log(line, "Error", className);
    }

    void Logger::debug(const std::string& line) {
        log(line, "Debug", className);
    }


    void Logger::soloDebugLog(const std::string& line) {
        log(line, "Debug");
    }


    std::string Logger::getLogFeed() {
        return logBuffer.str();
    }

    void Logger::clearLog() {
        logBuffer.str("");
        logBuffer.clear();
    }

    int Logger::getLogUpdateId() {
        return logUpdateId;
    }


    void Logger::log(const std::string& line, const std::string& callerName, const std::string& logLevel) {
        checkAndTrimLogger(line);
        bool prefixAdded = false;
        if (!callerName.empty()) {
            logBuffer << "[" + callerName + "]";
            Serial.print("[");
            Serial.print(callerName.c_str());
            Serial.print("]");
            prefixAdded = true;
        }
        if (!logLevel.empty()) {
            logBuffer << "[" + logLevel + "]";
            Serial.print("[");
            Serial.print(logLevel.c_str());
            Serial.print("]");
            prefixAdded = true;
        }

        if (prefixAdded) {
            logBuffer << " - ";
            Serial.print(" - ");
        }

        logBuffer << line << "\n";
        logUpdateId++;

        Serial.println(line.c_str());
    }

    /**
     * Check if new entry fits the buffer, if not trim the buffer
     */
    void Logger::checkAndTrimLogger(const std::string& line) {
        std::streamoff potentialNewSize =
                static_cast<std::streamoff>(logBuffer.tellp()) + static_cast<std::streamoff>(line.length());

        if (potentialNewSize > static_cast<std::streamoff>(maxLogBufferSize)) {
            size_t overSize = potentialNewSize - maxLogBufferSize;
            std::string currentLog = logBuffer.str();
            size_t trimPosition = currentLog.find('\n', overSize);
            if (trimPosition == std::string::npos) {
                // Couldn't find any newLine characters after the overflow
                logBuffer.str("");
                logBuffer.clear();
            } else {
                currentLog.erase(0, trimPosition + 1); // +1 to remove the newline character itself
                logBuffer.str(currentLog);
                logBuffer.clear();
            }
            // Setting ostringstream cursor back to end
            logBuffer.seekp(0, std::ios::end);
        }
    }



}

