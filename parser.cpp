#include "parser.h"
#include "ast.h" // <-- Make sure this is included
#include <stdexcept>
#include <iostream>

const Token& Parser::currentToken() const {
    if (pos < tokens.size()) return tokens[pos];
    static Token eofToken{TokenType::End, ""};
    return eofToken;
}

void Parser::consume(TokenType expected) {
    if (currentToken().type == expected) {
        pos++;
    } else {
        throw std::runtime_error("Unexpected token: " + currentToken().text);
    }
}

// program := (statement)* EOF
// statement := printStmt | assignStmt
/*std::vector<std::unique_ptr<Stmt>> Parser::parseProgram() {
    std::vector<std::unique_ptr<Stmt>> statements;

    while (currentToken().type != TokenType::End) {
        if (currentToken().type == TokenType::Print) {
            statements.push_back(parsePrintStmt());
        } else if (currentToken().type == TokenType::Identifier) {
            statements.push_back(parseAssignStmt());
        } else {
            throw std::runtime_error("Unknown statement starting with token: " + currentToken().text);
        }
    }

    return statements;
}*/
std::vector<std::unique_ptr<Stmt>> Parser::parseProgram() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (currentToken().type != TokenType::End) {
        if (currentToken().type == TokenType::IntType || currentToken().type == TokenType::FloatType || currentToken().type== TokenType::StringType) {
            statements.push_back(parseDeclaration());
        } else if (currentToken().type == TokenType::Identifier && tokens[pos + 1].type == TokenType::Equal) {
            statements.push_back(parseAssignStmt());
        } else if (currentToken().type == TokenType::Print) {
            statements.push_back(parsePrintStmt());
        }
        else if (currentToken().type == TokenType::IfType) {
            statements.push_back(parseIfStmt());
        }
        else if (currentToken().type == TokenType::WhileType) {
            statements.push_back(parseWhileStmt());
        } else {
            throw std::runtime_error("Unexpected token: " + currentToken().text);
        }
    }
    return statements;
}
std::unique_ptr<Stmt> Parser::parseDeclaration() {
    auto typeToken = currentToken();
    if (typeToken.type != TokenType::IntType &&
        typeToken.type != TokenType::FloatType &&
        typeToken.type != TokenType::StringType) {
        throw std::runtime_error("Expected type declaration");
    }
    consume(typeToken.type); // consume int, float, or string
    
    std::string name = currentToken().text;
    consume(TokenType::Identifier);

    consume(TokenType::Equal);
    auto expr = parseExpression();
    consume(TokenType::Semicolon);

    return std::make_unique<DeclStmt>(typeToken.type, name, std::move(expr));
}

// printStmt := 'print' '(' expression ')' ';'
std::unique_ptr<PrintStmt> Parser::parsePrintStmt() {
    consume(TokenType::Print);
    consume(TokenType::LParen);
    auto expr = parseExpression();
    consume(TokenType::RParen);
    consume(TokenType::Semicolon);
    return std::make_unique<PrintStmt>(std::move(expr));
}

// assignStmt := identifier '=' expression ';'
std::unique_ptr<AssignStmt> Parser::parseAssignStmt() {
    std::string name = currentToken().text;
    consume(TokenType::Identifier);
    consume(TokenType::Equal);
    auto expr = parseExpression();
    consume(TokenType::Semicolon);
    return std::make_unique<AssignStmt>(name, std::move(expr));
}


// expression := term (('+' | '-') term)*
/*std::unique_ptr<Expr> Parser::parseExpression() {
    auto left = parseTerm();

    while (currentToken().type == TokenType::Plus || currentToken().type == TokenType::Minus) {
        char op = currentToken().text[0];
        consume(currentToken().type);
        auto right = parseTerm();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}*/
std::unique_ptr<Expr> Parser::parseExpression() {
    return parseEquality();
}

std::unique_ptr<Expr> Parser::parseComparison() {
    auto left = parseAddition();
    while (currentToken().type == TokenType::Less ||
           currentToken().type == TokenType::LessEqual ||
           currentToken().type == TokenType::Greater ||
           currentToken().type == TokenType::GreaterEqual) {
        std::string opSymbol = currentToken().text;
        consume(currentToken().type);
        auto right = parseAddition();
        left = std::make_unique<BinaryExpr>(std::move(left), opSymbol, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseEquality() {
    auto left = parseComparison();
    while (currentToken().type == TokenType::DoubleEqual || currentToken().type == TokenType::BangEqual) {
        std::string opSymbol = currentToken().text;
        consume(currentToken().type);
        auto right = parseComparison();
        left = std::make_unique<BinaryExpr>(std::move(left), opSymbol, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::parseAddition() {
    auto left = parseTerm();

    while (currentToken().type == TokenType::Plus || currentToken().type == TokenType::Minus) {
        std::string opSymbol = currentToken().text;
        consume(currentToken().type);
        auto right = parseTerm();
        left = std::make_unique<BinaryExpr>(std::move(left), opSymbol, std::move(right));
    }

    return left;
}


// term := factor (('*' | '/') factor)*
std::unique_ptr<Expr> Parser::parseTerm() {
    auto left = parseFactor();

    while (currentToken().type == TokenType::Star || currentToken().type == TokenType::Slash || currentToken().type == TokenType::Percent) {
        std::string op = currentToken().text;
        consume(currentToken().type);
        auto right = parseFactor();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }

    return left;
}

// factor := NUMBER | identifier | '(' expression ')'
std::unique_ptr<Expr> Parser::parseFactor() {
    if (currentToken().type == TokenType::Number) {
        const std::string& txt = currentToken().text;
        if (txt.find('.') != std::string::npos) {
            double val = std::stod(txt);
            consume(TokenType::Number);
            return std::make_unique<NumberExpr>(val);
        } else {
            int val = std::stoi(txt);
            consume(TokenType::Number);
            return std::make_unique<NumberExpr>(val);
        }
    }
    else if (currentToken().type == TokenType::Identifier) {
        std::string name = currentToken().text;
        consume(TokenType::Identifier);
        return std::make_unique<VariableExpr>(name);
    }
    else if (currentToken().type == TokenType::LParen) {
        consume(TokenType::LParen);
        auto expr = parseExpression();
        consume(TokenType::RParen);
        return expr;
    }
    else if (currentToken().type == TokenType::StringLiteral) {
        std::string val = currentToken().text;
        consume(TokenType::StringLiteral);
        return std::make_unique<StringExpr>(val);
    }
    else {
        throw std::runtime_error("Unexpected token in factor: '" + currentToken().text +
            "' (type = " + std::to_string(static_cast<int>(currentToken().type)) + ")");
    }
}

std::unique_ptr<IfStmt> Parser::parseIfStmt() {
    consume(TokenType::IfType);
    consume(TokenType::LParen);
    
    auto condition = parseExpression();
    consume(TokenType::RParen);
    // Parse the 'then' branch
    auto thenBranch = parseStatement();

    // Optional 'else' branch
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (currentToken().type == TokenType::ElseType) {
        consume(TokenType::ElseType);
        elseBranch = parseStatement();
    }

    return std::make_unique<IfStmt>(
        std::move(condition),
        std::move(thenBranch),
        std::move(elseBranch)
    );



}
std::unique_ptr<WhileStmt> Parser::parseWhileStmt() {
    consume(TokenType::WhileType);
    consume(TokenType::LParen);
    
    auto condition = parseExpression();
    consume(TokenType::RParen);
    auto body = parseStatement();

    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Stmt> Parser::parseStatement() {
    if (currentToken().type == TokenType::Print) {
        return parsePrintStmt();
    }
    else if (currentToken().type == TokenType::IfType) {
        return parseIfStmt();
    }
    else if (currentToken().type == TokenType::WhileType) {
        return parseWhileStmt();
    }
    else if (currentToken().type == TokenType::LBrace) {
        return parseBlockStmt();
    }
    else if (currentToken().type == TokenType::IntType ||
             currentToken().type == TokenType::FloatType ||
             currentToken().type == TokenType::StringType) {
        return parseDeclaration();
    }
    else if (currentToken().type == TokenType::Identifier &&
             (pos + 1 < tokens.size()) && tokens[pos + 1].type == TokenType::Equal) { // <-- Fix here
        return parseAssignStmt();
    }
    else {
        throw std::runtime_error("Unexpected token in statement: " + currentToken().text);
    }
}
std::unique_ptr<Stmt> Parser::parseBlockStmt() {
    consume(TokenType::LBrace);
    std::vector<std::unique_ptr<Stmt>> stmts;
    while (currentToken().type != TokenType::RBrace && currentToken().type != TokenType::End) {
        stmts.push_back(parseStatement());
    }
    consume(TokenType::RBrace);
    return std::make_unique<BlockStmt>(std::move(stmts));
}

