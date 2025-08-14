#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Number, Identifier, Plus, Minus, Star, Slash,
    LParen, RParen, Semicolon, Equal, IntType, FloatType, End, Print, StringType,  // Added StringType
    StringLiteral, IfType, DoubleEqual, BangEqual, ElseType
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
public:
    explicit Lexer(const std::string &src) : source(src) {}
    std::vector<Token> tokenize();
private:
    std::string source;
    size_t pos = 0;
    char peek() const;
    char get();
    void skipWhitespace();
    bool isAtEnd() const;
};
