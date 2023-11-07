#ifndef ARDUINO_COMMANDUTILS_H
#define ARDUINO_COMMANDUTILS_H

#include <sstream>


namespace CommandUtils {
    extern std::string parseNextWord(std::istringstream& istringstream);
};


#endif //ARDUINO_COMMANDUTILS_H
