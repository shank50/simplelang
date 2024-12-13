#pragma once

#include "ast.h"
#include "token.h"

#include <vector>

class Parser {
public:
    Parser(std::vector<Token> tokens);

    Program parse();

private:
    bool match(TokenType type);
    bool check(TokenType type) const;
    const Token& advanceToken();
    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;

    StmtPtr statement();
    StmtPtr letStatement();
    StmtPtr printStatement();

    ExprPtr expression();
    ExprPtr addition();
    ExprPtr multiplication();
    ExprPtr unary();
    ExprPtr primary();

    void consume(TokenType type, const char* message);

    std::vector<Token> m_tokens;
    size_t m_current = 0;
};
