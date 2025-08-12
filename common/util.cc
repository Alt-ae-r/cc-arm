#include <fstream>
#include <sstream>

#include "util.h"

namespace util {

bool matchStr(const std::string &body, std::string searchStr, size_t index) {
    if (body.size() - index < searchStr.size()) {
        return false;
    }
    return body.substr(index, searchStr.size()) == searchStr;
}

std::string readEntireFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> splitStr(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

} // namespace util