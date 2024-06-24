#include "TextUtils.h"

namespace TextUtils {

    std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
        std::string::size_type start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

}