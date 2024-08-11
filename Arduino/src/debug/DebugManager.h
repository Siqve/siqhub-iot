#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include "Constants.h"
#include "Logger.h"
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
    void registerDebugCommandListener(const std::string& cmd, std::function<void(std::string&)> callback);

private:
    DebugManager() : logger(Debug::Logger("DebugManager")) {};

    std::map<std::string, std::function<void(std::string&)>> debugCommandListeners;
    Debug::Logger logger;
};


#endif //CUSTOMLOGGER_H
