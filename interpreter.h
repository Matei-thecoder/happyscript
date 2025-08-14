#pragma once

#include "ast.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <memory>

class Interpreter {
public:
    void interpret(const std::vector<std::unique_ptr<Stmt>>& program);

private:
    void execute(const Stmt* stmt);
    std::variant<int, double, std::string> evaluate(const Expr* expr);
    // Variables can be int or double, stored in a variant
    std::unordered_map<std::string, std::variant<int, double, std::string>> variables;

};
