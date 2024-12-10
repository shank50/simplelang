#pragma once

#include <string>

enum class TokenType {
    Let,
    Print,
    Identifier,
    Number,
    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    Semicolon,
    LeftParen,
    RightParen,
    EndOfFile
};

struct Token {
    TokenType type;
    std::string lexeme;
    double numberValue = 0.0;
};
