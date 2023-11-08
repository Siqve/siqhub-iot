#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include "Constants.h"
#include <sstream>
#include <utility>
#include <map>

class DebugManager {
public:
    // Deleted copy constructor and copy assignment operator
    DebugManager(const DebugManager&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;

    // Static method to access Singleton instance
    static DebugManager& getInstance() {
        static DebugManager instance;  // Guaranteed to be lazy initialized and destroyed correctly
        return instance;
    }

    void onDebugCommand(const std::string& command);
    void logDebug(const std::string& line);

    void registerDebugCommandListener(const std::string& cmd, std::function<void(std::string&)> callback);

    class Logger {
    public:
        Logger(DebugManager& debugManager, std::string className) : debugManager(debugManager),
                                                                           className(std::move(className)) {};

        void log(const std::string& line, const std::string& logType = "");
        void info(const std::string& line);
        void warn(const std::string& line);
        void error(const std::string& line);
        void debug(const std::string& line);

        std::string getLogFeed();
        void clearLog();
        int getLogUpdateId();
    private:
        DebugManager& debugManager;
        const std::string className;
    };

    Logger newLogger(const std::string& className = "");
private:
    DebugManager() : logger(Logger(*this, "DebugManager")) {};

    void checkAndTrimLogger(const std::string& line);

    std::map<std::string, std::function<void(std::string&)>> debugCommandListeners;
    Logger logger;
    int maxLogBufferSize = DebugConstants::MAX_LOG_BUFFER_SIZE;
    std::ostringstream logBuffer;
    int logUpdateId = 0;
};


#endif //CUSTOMLOGGER_H
