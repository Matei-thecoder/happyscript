#include "lexer.h"
#include <cctype>
#include <stdexcept>

char Lexer::peek() const {
    return pos < source.size() ? source[pos] : '\0';
}

char Lexer::get() {
    return pos < source.size() ? source[pos++] : '\0';
}

void Lexer::skipWhitespace() {
    while (std::isspace(peek())) get();
}

bool Lexer::isAtEnd() const {
    return pos >= source.size();
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    skipWhitespace();
       
    while (pos < source.size()) {
        char c = peek();
        
        if (std::isdigit(c)) {
            std::string num;
            while (std::isdigit(peek()) || peek() == '.') num += get();
            tokens.push_back({TokenType::Number, num});
        }
        else if (c == '"') {
            get();
            std::string str;
            while (peek() != '"' && !isAtEnd()) {
                str += get();
            }
            if (get() != '"') {
                throw std::runtime_error("Unterminated string literal");
            }
            tokens.push_back({ TokenType::StringLiteral, str });
        }
        else if (c == '\'') {
            get(); // consume opening quote
            char ch = get();
            if (peek() == '\\') { // handle escape sequences
                get(); // consume '\'
                ch = get(); // get escaped character
            }
            if (get() != '\'') {
                throw std::runtime_error("Unterminated character literal");
            }
            tokens.push_back({ TokenType::StringLiteral, std::string(1, ch) });
        }
        else if (std::isalpha(c)) {
            std::string id;
            while (std::isalnum(peek())) id += get();
            if (id == "smile") tokens.push_back({ TokenType::Print, id });
            else if (id == "int") tokens.push_back({ TokenType::IntType, id });
            else if (id == "float") tokens.push_back({ TokenType::FloatType, id });
            else if (id == "string") tokens.push_back({ TokenType::StringType, id });
            else if (id == "ana") tokens.push_back({ TokenType::IfType,id });
            else if (id == "elsa") tokens.push_back({ TokenType::ElseType,id });
            else tokens.push_back({TokenType::Identifier, id});
        }
        
        
        else {
            switch (c) {
                case '+': tokens.push_back({TokenType::Plus, std::string(1,get())}); break;
                case '-': tokens.push_back({TokenType::Minus, std::string(1,get())}); break;
                case '*': tokens.push_back({TokenType::Star, std::string(1,get())}); break;
                case '/': tokens.push_back({TokenType::Slash, std::string(1,get())}); break;
                case '(': tokens.push_back({TokenType::LParen, std::string(1,get())}); break;
                case ')': tokens.push_back({TokenType::RParen, std::string(1,get())}); break;
                case ';': tokens.push_back({TokenType::Semicolon, std::string(1,get())}); break;
                case '=':
                    get(); // consume '='
                    if (peek() == '=') {
                        get(); // consume second '='
                        tokens.push_back({TokenType::DoubleEqual, "=="});
                    } else {
                        tokens.push_back({TokenType::Equal, "="});
                    }
                    break;
                case '!':
                    get(); // consume '!'
                    if (peek() == '=') {
                        get(); // consume '='
                        tokens.push_back({TokenType::BangEqual, "!="});
                    } else {
                        // handle single '!' if needed
                    }
                    break;
                default: get(); break; // skip unknown
            }
        }
        skipWhitespace();
    }
    tokens.push_back({TokenType::End, ""});
    return tokens;
}
