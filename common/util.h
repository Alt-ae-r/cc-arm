#pragma once

#include <string>

namespace util {

bool matchStr(std::string &body, std::string searchStr, size_t index = 0);

std::string readEntireFile(const std::string &filename);

} // namespace util