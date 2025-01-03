#include "TextUtils.h"
#include <sstream>

namespace TextUtils {

    std::string replaceAll(std::string str, const std::string &from, const std::string &to) {
        std::string::size_type start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    std::string parseNextWord(std::istringstream &istringstream) {
        istringstream >> std::ws;
        std::string nextWord;
        istringstream >> nextWord;
        return nextWord;
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> result;
        std::istringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            result.push_back(item);
        }
        return result;
    }

}