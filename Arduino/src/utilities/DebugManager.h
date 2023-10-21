#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include "Arduino.h"
#include <sstream>

class DebugManager {
public:
    void onDebug(const std::string& command);

    void logLine(const std::string& line, const std::string& logType = "info");
    void logLine(const String& line, const String& logType = "info");
    std::string getLogFeed();
    void clearLog();
    int getLogUpdateId();
private:
    std::ostringstream logBuffer;
    int logUpdateId = 0;
};


#endif //CUSTOMLOGGER_H
