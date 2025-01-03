#pragma once

#include <string>
#include <vector>

namespace TextUtils {

    std::string replaceAll(std::string str, const std::string &from, const std::string &to);
    std::string parseNextWord(std::istringstream &istringstream);
    std::vector<std::string> split(const std::string &s, char delim);

}