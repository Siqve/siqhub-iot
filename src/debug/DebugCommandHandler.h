#pragma once

#include "Logger.h"
#include <map>
#include <functional>

namespace Debug {

    class DebugCommandHandler {
    public:
        DebugCommandHandler(const DebugCommandHandler &) = delete;
        DebugCommandHandler &operator=(const DebugCommandHandler &) = delete;

        static DebugCommandHandler &getInstance() {
            static DebugCommandHandler instance;
            return instance;
        }

        void initialize();

        bool execute(const std::string &command);
        void registerListener(const std::string &cmd, std::function<void(std::string &)> callback);
        void removeListener(const std::string &cmd);

    private:
        DebugCommandHandler() = default;

        std::map<std::string, std::function<void(std::string &)>> commandListeners;
        Debug::Logger logger = Debug::Logger("DebugCommandHandler");
    };

}