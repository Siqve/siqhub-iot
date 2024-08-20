#ifndef CUSTOMLOGGER_H
#define CUSTOMLOGGER_H

#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include "Constants.h"
#include "Logger.h"
#include "web/WebServerManager.h"
#include <utility>
#include <map>

namespace Debug {

    class DebugCommandHandler {
    public:
        DebugCommandHandler(const DebugCommandHandler&) = delete;
        DebugCommandHandler& operator=(const DebugCommandHandler&) = delete;

        static DebugCommandHandler& getInstance() {
            static DebugCommandHandler instance;  // Guaranteed to be lazy initialized and destroyed correctly
            return instance;
        }

        bool execute(const std::string& command);
        void registerListener(const std::string& cmd, std::function<void(std::string&)> callback);

    private:
        DebugCommandHandler() : logger(Debug::Logger("DebugCommandHandler")) {
            WebServerManager::getInstance().registerPageCallback("/debug", [](const RequestWrapper& request) {
                String cmd = request.getParam("cmd")->value();
                if (!Debug::DebugCommandHandler::getInstance().execute(cmd.c_str())) {
                    return request.notFound();
                }
                return request.ok();
            });

        };

        std::map<std::string, std::function<void(std::string&)>> commandListeners;
        Debug::Logger logger;
    };

}

#endif //CUSTOMLOGGER_H
