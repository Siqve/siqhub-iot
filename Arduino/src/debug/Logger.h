#ifndef ARDUINO_LOGGER_H
#define ARDUINO_LOGGER_H

#include "Arduino.h"
#include <sstream>


namespace Debug {
    class Logger {
    public:
        explicit Logger(std::string className) : className(std::move(className)) {};

        static std::string getLogFeed();
        static void clearLog();
        static int getLogUpdateId();
        static void soloDebugLog(const std::string& line);

        void info(const std::string& line);
        void warn(const std::string& line);
        void error(const std::string& line);
        void debug(const std::string& line);

    private:
        static void checkAndTrimLogger(const std::string& line);
        static void log(const std::string& line, const std::string& logLevel = "", const std::string& callerName = "");

        static std::ostringstream logBuffer;
        static int logUpdateId;
        static int maxLogBufferSize;

        const std::string className;

    };
}


#endif //ARDUINO_LOGGER_H
