#include "CommandUtils.h"

std::string CommandUtils::parseNextWord(std::istringstream& istringstream) {
    istringstream >> std::ws;
    std::string nextWord;
    istringstream >> nextWord;
    return nextWord;
}
