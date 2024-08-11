#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include "Constants.h"
#include "Logger.h"
#include <utility>
#include <map>

namespace Debug {

    class DebugCommandHandler {
    public:
        // Deleted copy constructor and copy assignment operator
        DebugCommandHandler(const DebugCommandHandler&) = delete;
        DebugCommandHandler& operator=(const DebugCommandHandler&) = delete;

        // Static method to access Singleton instance
        static DebugCommandHandler& getInstance() {
            static DebugCommandHandler instance;  // Guaranteed to be lazy initialized and destroyed correctly
            return instance;
        }

        void execute(const std::string& command);
        void registerListener(const std::string& cmd, std::function<void(std::string&)> callback);

    private:
        DebugCommandHandler() : logger(Debug::Logger("DebugCommandHandler")) {};

        std::map<std::string, std::function<void(std::string&)>> commandListeners;
        Debug::Logger logger;
    };

}

#endif //CUSTOMLOGGER_H
