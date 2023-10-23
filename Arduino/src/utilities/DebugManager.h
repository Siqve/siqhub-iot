#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include <sstream>

class DebugManager {
public:
    DebugManager() : loggerObject(Logger()) {};
    void onDebug(const std::string& command);

    class Logger {
    public:
        void log(const std::string& line, const std::string& logType);
        void info(const std::string& line);
        void warn(const std::string& line);
        void error(const std::string& line);
        void debug(const std::string& line);

        std::string getLogFeed();
        void clearLog();
        int getLogUpdateId();
    private:
        std::ostringstream logBuffer;
        int logUpdateId = 0;
    };

    Logger& logger();

private:
    Logger loggerObject;
};


#endif //CUSTOMLOGGER_H
