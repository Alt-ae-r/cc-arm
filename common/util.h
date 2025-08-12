#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace util {

bool matchStr(const std::string &body, std::string searchStr, size_t index = 0);

std::string readEntireFile(const std::string &filename);

inline std::string readEntireFile(const std::filesystem::path &filename) {
    return readEntireFile(filename.string());
};

std::vector<std::string> splitStr(const std::string &str, char delimiter);

} // namespace util