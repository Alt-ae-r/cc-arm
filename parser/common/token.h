#pragma once

#include <format>
#include <string>

enum TokenKind {
    KEYWORD,
    LITERAL,
    LEFT_PAREN,
    LEFT_BRACE,
    LEFT_BRACKET,
    RIGHT_PAREN,
    RIGHT_BRACE,
    RIGHT_BRACKET,
    SEMICOLON,
    IDENTIFIER,
};

inline std::string stringifyTokenKind(TokenKind kind) {
    switch (kind) {
    case KEYWORD:
        return "KEYWORD";
    case LITERAL:
        return "LITERAL";
    case LEFT_PAREN:
        return "LEFT_PAREN";
    case LEFT_BRACE:
        return "LEFT_BRACE";
    case LEFT_BRACKET:
        return "LEFT_BRACKET";
    case RIGHT_PAREN:
        return "RIGHT_PAREN";
    case RIGHT_BRACE:
        return "RIGHT_BRACE";
    case RIGHT_BRACKET:
        return "RIGHT_BRACKET";
    case SEMICOLON:
        return "SEMICOLON";
    case IDENTIFIER:
        return "IDENTIFIER";
    default:
        return "UNKNOWN";
    }
}

struct Token {
    TokenKind kind;
    std::string str;

    Token(TokenKind kind, std::string str) : kind{kind}, str{str} {}
    Token(TokenKind kind, char c) : kind{kind}, str{c} {}

    std::string string() { 
        return std::format("{}\t{}", stringifyTokenKind(kind), str);
    }
};
