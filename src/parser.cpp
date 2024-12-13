#include "parser.h"

#include <stdexcept>

Parser::Parser(std::vector<Token> tokens) {
    m_tokens = tokens;
}

Program Parser::parse() {
    Program program;
    while (!isAtEnd()) {
        program.push_back(statement());
    }
    return program;
}

StmtPtr Parser::statement() {
    if (match(TokenType::Let)) {
        return letStatement();
    }
    if (match(TokenType::Print)) {
        return printStatement();
    }
    throw std::runtime_error("Expected statement");
}

StmtPtr Parser::letStatement() {
    const Token& nameToken = peek();
    consume(TokenType::Identifier, "Expected identifier after 'let'");
    consume(TokenType::Equal, "Expected '=' in let statement");
    ExprPtr value = expression();
    consume(TokenType::Semicolon, "Expected ';' after let statement");
    return std::make_unique<LetStmt>(nameToken.lexeme, std::move(value));
}

StmtPtr Parser::printStatement() {
    ExprPtr value = expression();
    consume(TokenType::Semicolon, "Expected ';' after print statement");
    return std::make_unique<PrintStmt>(std::move(value));
}

ExprPtr Parser::expression() {
    return addition();
}

ExprPtr Parser::addition() {
    ExprPtr expr = multiplication();
    while (check(TokenType::Plus) || check(TokenType::Minus)) {
        TokenType op = peek().type;
        advanceToken();
        ExprPtr right = multiplication();
        BinaryOp binOp = (op == TokenType::Plus) ? BinaryOp::Add : BinaryOp::Sub;
        expr = std::make_unique<BinaryExpr>(binOp, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::multiplication() {
    ExprPtr expr = unary();
    while (check(TokenType::Star) || check(TokenType::Slash)) {
        TokenType op = peek().type;
        advanceToken();
        ExprPtr right = unary();
        BinaryOp binOp = (op == TokenType::Star) ? BinaryOp::Mul : BinaryOp::Div;
        expr = std::make_unique<BinaryExpr>(binOp, std::move(expr), std::move(right));
    }
    return expr;
}

ExprPtr Parser::unary() {
    if (match(TokenType::Minus)) {
        ExprPtr right = unary();
        return std::make_unique<BinaryExpr>(BinaryOp::Sub, std::make_unique<NumberExpr>(0.0), std::move(right));
    }
    if (match(TokenType::Plus)) {
        return unary();
    }
    return primary();
}

ExprPtr Parser::primary() {
    if (match(TokenType::Number)) {
        return std::make_unique<NumberExpr>(previous().numberValue);
    }
    if (match(TokenType::Identifier)) {
        return std::make_unique<VariableExpr>(previous().lexeme);
    }
    if (match(TokenType::LeftParen)) {
        ExprPtr expr = expression();
        consume(TokenType::RightParen, "Expected ')' after expression");
        return expr;
    }
    throw std::runtime_error("Expected expression");
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advanceToken();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) {
        return false;
    }
    return peek().type == type;
}

const Token& Parser::advanceToken() {
    if (!isAtEnd()) {
        ++m_current;
    }
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EndOfFile;
}

const Token& Parser::peek() const {
    return m_tokens[m_current];
}

const Token& Parser::previous() const {
    return m_tokens[m_current - 1];
}

void Parser::consume(TokenType type, const char* message) {
    if (check(type)) {
        advanceToken();
        return;
    }
    throw std::runtime_error(message);
}
