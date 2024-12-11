#pragma once

#include "token.h"

#include <string>
#include <vector>

class Lexer {
public:
    Lexer(std::string source);

    std::vector<Token> tokenize();

private:
    char peek() const;
    char peekNext() const;
    char advance();
    bool match(char expected);
    void skipWhitespace();

    void scanToken();
    void identifier();
    void number();

    void addToken(TokenType type);
    void addToken(TokenType type, double numberValue);

    std::string m_source;
    std::vector<Token> m_tokens;
    size_t m_current = 0;
};
