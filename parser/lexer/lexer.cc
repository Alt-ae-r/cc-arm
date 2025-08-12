#include <cassert>
#include <functional>
#include <optional>

#include "common/util.h"
#include "lexer.h"
#include "parser/common/token.h"

namespace lexer {

static const auto KEYWORD_NAMES = {std::string("return"), std::string("int")};

std::optional<Token> parseKeyword(const std::string &body, size_t &index) {
    for (auto keyword : KEYWORD_NAMES) {
        if (util::matchStr(body, keyword, index)) {
            index += keyword.size();
            return Token{TokenKind::KEYWORD, keyword};
        }
    }
    return std::nullopt;
}

std::string parseMatchingChars(const std::string &body, size_t &index, std::function<bool(char)> eval) {
    auto startIndex = index;
    while (index < body.size() && eval(body[index])) {
        index++;
    }
    return body.substr(startIndex, index - startIndex);
}

bool isValidIdentifierChar(char c) {
    return (c >= 'A' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
}

std::optional<Token> parseIdentifier(const std::string &body, size_t &index) {
    if (char c = body[index]; (c < 'A' || c > 'z') && c != '_') {
        return std::nullopt;
    }
    auto id = parseMatchingChars(body, index, isValidIdentifierChar);
    assert(id.size() > 0);
    return Token{TokenKind::IDENTIFIER, id};
}

bool parseWhitespace(const std::string &body, size_t &index) {
    return parseMatchingChars(body, index, [](char c) {
               return c == '\t' || c == '\n' || c == ' ';
           }).size() > 0;
}

std::optional<Token> classifySingleCharToken(char c) {
    switch (c) {
    case '(':
        return Token{TokenKind::LEFT_PAREN, std::string(1, c)};
    case ')':
        return Token{TokenKind::RIGHT_PAREN, std::string(1, c)};
    case '[':
        return Token{TokenKind::LEFT_BRACKET, std::string(1, c)};
    case ']':
        return Token{TokenKind::RIGHT_BRACKET, std::string(1, c)};
    case '{':
        return Token{TokenKind::LEFT_BRACE, std::string(1, c)};
    case '}':
        return Token{TokenKind::RIGHT_BRACE, std::string(1, c)};
    case ';':
        return Token{TokenKind::SEMICOLON, std::string(1, c)};
    default:
        return std::nullopt;
    }
}

// parse any token guaranteed to be only a single char
std::optional<Token> parseSingleCharToken(const std::string &body, size_t &index) {
    auto c = body[index];
    auto output = classifySingleCharToken(c);

    if (output) {
        index++;
    }
    return output;
}

bool isNumeric(char c) { return '0' <= c && c <= '9'; }

std::optional<Token> parseNumber(const std::string &body, size_t &index) {
    auto literal = parseMatchingChars(body, index, isNumeric);
    assert(literal.size() > 0);
    return Token{TokenKind::LITERAL, literal};
}

std::optional<Token> parseLiteral(const std::string &body, size_t &index) {
    if (isNumeric(body[index])) {
        auto token = parseNumber(body, index);
        assert(token &&
               "tokens starting with a numeric character should be valid numeric literals.");
        return token;
    }
    return std::nullopt;
}

std::vector<Token> tokenizeProgram(const std::string &body /* program body */) {
    auto tokens = std::vector<Token>{};
    size_t index = 0;
    while (index < body.size()) {
        if (parseWhitespace(body, index)) {
            continue;
        }
        if (auto token = parseSingleCharToken(body, index)) {
            tokens.push_back(*token);
        } else if (auto token = parseKeyword(body, index)) {
            tokens.push_back(*token);
        } else if (auto token = parseIdentifier(body, index)) {
            tokens.push_back(*token);
        } else if (auto token = parseLiteral(body, index)) {
            tokens.push_back(*token);
        }
    }

    return tokens;
}
} // namespace lexer