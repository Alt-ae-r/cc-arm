#pragma once

#include <vector>

#include "../common/token.h"

namespace lexer {
    std::vector<Token> tokenizeProgram(const std::string &body /* program body */);
}