#pragma once
#include <string>
#include <memory>
#include "lexer.h"
// Base class for expressions
struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    double value;
    NumberExpr(double v) : value(v) {}
};

struct VariableExpr : Expr {
    std::string name;
    VariableExpr(const std::string& n) : name(n) {}
};

struct BinaryExpr : Expr {
    std::unique_ptr<Expr> left, right;
    std::string  op;
    BinaryExpr(std::unique_ptr<Expr> l, std::string o, std::unique_ptr<Expr> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
};

// Base class for statements
struct Stmt {
    virtual ~Stmt() = default;
};

// Print statement
struct PrintStmt : Stmt {
    std::unique_ptr<Expr> expr;
    PrintStmt(std::unique_ptr<Expr> e) : expr(std::move(e)) {}
};
struct IfStmt : Stmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    IfStmt(std::unique_ptr<Expr> cond,
        std::unique_ptr<Stmt> thenStmt,
        std::unique_ptr<Stmt> elseStmt = nullptr)
        : condition(std::move(cond)),
        thenBranch(std::move(thenStmt)),
        elseBranch(std::move(elseStmt)) {}
};


// Assignment statement
struct AssignStmt : Stmt {
    std::string name;
    std::unique_ptr<Expr> value;
    AssignStmt(const std::string& n, std::unique_ptr<Expr> v)
        : name(n), value(std::move(v)) {}
};
struct DeclStmt : Stmt {
    TokenType varType;
    std::string name;
    std::unique_ptr<Expr> value;

    DeclStmt(TokenType varType, const std::string& name, std::unique_ptr<Expr> value)
        : varType(varType), name(name), value(std::move(value)) {}
};
struct StringExpr : Expr {
    std::string value;
    StringExpr(const std::string& value) : value(std::move(value)) {}
};

