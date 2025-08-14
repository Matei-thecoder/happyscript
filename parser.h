#pragma once
#include "lexer.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    // Parse a full program (list of statements)
    std::vector<std::unique_ptr<Stmt>> parseProgram();

private:
    const std::vector<Token>& tokens;
    size_t pos = 0;

    const Token& currentToken() const;
    void consume(TokenType expected);

    std::unique_ptr<PrintStmt> parsePrintStmt();
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<AssignStmt> parseAssignStmt();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Stmt> parseDeclaration();
    std::unique_ptr<IfStmt> parseIfStmt();
    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Expr> parseEquality();
    std::unique_ptr<Expr> parseAddition();
    
    
};
