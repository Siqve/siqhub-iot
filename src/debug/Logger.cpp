#include <HardwareSerial.h>
#include "Logger.h"


#ifdef ESP8266
#define LOG_BUFFER_SIZE 2048
#else
#define LOG_BUFFER_SIZE 4096
#endif

namespace Debug {

    std::ostringstream Logger::logBuffer;
    int Logger::logUpdateId = 0;
    int Logger::maxLogBufferSize = LOG_BUFFER_SIZE;


    void Logger::info(const std::string &line) {
        log(line, "INFO", className);
    }

    void Logger::warn(const std::string &line) {
        log(line, "WARN", className);
    }

    void Logger::error(const std::string &line) {
        log(line, "ERROR", className);
    }

    void Logger::debug(const std::string &line) {
        log(line, "DEBUG", className);
    }


    void Logger::soloDebugLog(const std::string &line) {
        log(line, "DEBUG");
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

    void Logger::log(const std::string &line, const std::string &logLevel, const std::string &callerName) {
        checkAndTrimLogger(line);
        bool prefixAdded = false;
        if (!logLevel.empty()) {
            logBuffer << logLevel;
            Serial.print(logLevel.c_str());
            prefixAdded = true;
        }

        if (!callerName.empty()) {
            if (prefixAdded) {
                logBuffer << " -- ";
                Serial.print(" -- ");
            }
            logBuffer << "[" + callerName + "]";
            Serial.print("[");
            Serial.print(callerName.c_str());
            Serial.print("]");
            prefixAdded = true;
        }


        if (prefixAdded) {
            logBuffer << ": ";
            Serial.print(": ");
        }

        logBuffer << line << "\n";
        logUpdateId++;

        Serial.println(line.c_str());
    }

    /**
     * Check if new entry fits the buffer, if not trim the buffer
     */
    void Logger::checkAndTrimLogger(const std::string &line) {
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

