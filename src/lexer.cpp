#include "lexer.h"

#include <cctype>
#include <stdexcept>

Lexer::Lexer(std::string source) {
    m_source = source;
}

std::vector<Token> Lexer::tokenize() {
    while (m_current < m_source.size()) {
        skipWhitespace();
        if (m_current >= m_source.size()) {
            break;
        }
        scanToken();
    }
    m_tokens.push_back({TokenType::EndOfFile, "", 0.0});
    return m_tokens;
}

char Lexer::peek() const {
    if (m_current >= m_source.size()) {
        return '\0';
    }
    return m_source[m_current];
}

char Lexer::peekNext() const {
    if (m_current + 1 >= m_source.size()) {
        return '\0';
    }
    return m_source[m_current + 1];
}

char Lexer::advance() {
    return m_source[m_current++];
}

bool Lexer::match(char expected) {
    if (peek() != expected) {
        return false;
    }
    ++m_current;
    return true;
}

void Lexer::skipWhitespace() {
    while (m_current < m_source.size()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t' || c == '\n') {
            ++m_current;
        } else {
            break;
        }
    }
}

void Lexer::scanToken() {
    char c = advance();
    if (std::isalpha(c) || c == '_') {
        identifier();
        return;
    }
    if (std::isdigit(c)) {
        number();
        return;
    }

    switch (c) {
    case '+':
        addToken(TokenType::Plus);
        break;
    case '-':
        addToken(TokenType::Minus);
        break;
    case '*':
        addToken(TokenType::Star);
        break;
    case '/':
        addToken(TokenType::Slash);
        break;
    case '=':
        addToken(TokenType::Equal);
        break;
    case ';':
        addToken(TokenType::Semicolon);
        break;
    case '(':
        addToken(TokenType::LeftParen);
        break;
    case ')':
        addToken(TokenType::RightParen);
        break;
    default:
        throw std::runtime_error(std::string("Unexpected character: ") + c);
    }
}

void Lexer::identifier() {
    size_t start = m_current - 1;
    while (std::isalnum(peek()) || peek() == '_') {
        advance();
    }
    std::string text = m_source.substr(start, m_current - start);
    if (text == "let") {
        addToken(TokenType::Let);
    } else if (text == "print") {
        addToken(TokenType::Print);
    } else {
        m_tokens.push_back({TokenType::Identifier, text, 0.0});
    }
}

void Lexer::number() {
    size_t start = m_current - 1;
    while (std::isdigit(peek())) {
        advance();
    }
    if (peek() == '.' && std::isdigit(peekNext())) {
        advance();
        while (std::isdigit(peek())) {
            advance();
        }
    }
    double value = std::stod(m_source.substr(start, m_current - start));
    addToken(TokenType::Number, value);
}

void Lexer::addToken(TokenType type) {
    m_tokens.push_back({type, "", 0.0});
}

void Lexer::addToken(TokenType type, double numberValue) {
    m_tokens.push_back({type, "", numberValue});
}
