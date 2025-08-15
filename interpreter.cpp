#include "interpreter.h"
#include <stdexcept>
#include <iostream>
#include <variant>
#include <string>

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& program) {
    for (const auto& stmt : program) {
        execute(stmt.get());
    }
}

void Interpreter::execute(const Stmt* stmt) {
    if (auto printStmt = dynamic_cast<const PrintStmt*>(stmt)) {
        auto val = evaluate(printStmt->expr.get());
        std::visit([](auto&& arg) { std::cout << arg << std::endl; }, val);
    }
    else if (auto assignStmt = dynamic_cast<const AssignStmt*>(stmt)) {
        auto val = evaluate(assignStmt->value.get());
        variables[assignStmt->name] = val;
    }
    else if (auto declStmt = dynamic_cast<const DeclStmt*>(stmt)) {
        auto val = evaluate(declStmt->value.get());

        if (declStmt->varType == TokenType::IntType) {
            if (auto pInt = std::get_if<int>(&val)) variables[declStmt->name] = *pInt;
            else if (auto pDouble = std::get_if<double>(&val)) variables[declStmt->name] = static_cast<int>(*pDouble);
            else throw std::runtime_error("Type mismatch assigning to int variable");
        }
        else if (declStmt->varType == TokenType::FloatType) {
            if (auto pInt = std::get_if<int>(&val)) variables[declStmt->name] = static_cast<double>(*pInt);
            else if (auto pDouble = std::get_if<double>(&val)) variables[declStmt->name] = *pDouble;
            else throw std::runtime_error("Type mismatch assigning to float variable");
        }
        else if (declStmt->varType == TokenType::StringType) {
            if (auto pStr = std::get_if<std::string>(&val)) variables[declStmt->name] = *pStr;
            else throw std::runtime_error("Type mismatch assigning to string variable");
        }
        else {
            throw std::runtime_error("Unknown variable type");
        }
    }
    else if (auto ifStmt = dynamic_cast<const IfStmt*>(stmt)) {
        auto cond = evaluate(ifStmt->condition.get());
        bool condVal = false;
        if (auto pInt = std::get_if<int>(&cond)) condVal = (*pInt != 0);
        else if (auto pDouble = std::get_if<double>(&cond)) condVal = (*pDouble != 0.0);
        else throw std::runtime_error("Condition must be numeric");
        if (condVal) {
            if (ifStmt->thenBranch)
                execute(ifStmt->thenBranch.get());
        } else {
            if (ifStmt->elseBranch)
                execute(ifStmt->elseBranch.get());
        }
    }
    else {
        throw std::runtime_error("Unknown statement type in execute");
    }
}

std::variant<int, double, std::string> Interpreter::evaluate(const Expr* expr) {
    if (auto n = dynamic_cast<const NumberExpr*>(expr)) {
        return n->value;
    }
    else if (auto v = dynamic_cast<const VariableExpr*>(expr)) {
        auto it = variables.find(v->name);
        if (it == variables.end()) throw std::runtime_error("Undefined variable: " + v->name);
        return it->second;
    }
    else if (auto s = dynamic_cast<const StringExpr*>(expr)) {
        return s->value;
    }
    else if (auto b = dynamic_cast<const BinaryExpr*>(expr)) {
        auto left = evaluate(b->left.get());
        auto right = evaluate(b->right.get());

        // Helper lambdas
        auto is_number = [](auto&& val) {
            return std::holds_alternative<int>(val) || std::holds_alternative<double>(val);
            };
        auto to_double = [](auto&& val) -> double {
            if (auto pInt = std::get_if<int>(&val)) return static_cast<double>(*pInt);
            if (auto pDouble = std::get_if<double>(&val)) return *pDouble;
            throw std::runtime_error("Value is not a number");
            };
        auto to_string = [](auto&& val) -> std::string {
            if (auto pStr = std::get_if<std::string>(&val)) return *pStr;
            throw std::runtime_error("Value is not a string");
            };

        // String concatenation for '+'
        if (b->op == "+" && (!is_number(left) || !is_number(right))) {
            return to_string(left) + to_string(right);
        }

        // Numeric operations
        if (!is_number(left) || !is_number(right)) {
            throw std::runtime_error("Cannot mix numbers and strings in arithmetic operations");
        }

        double leftVal = to_double(left);
        double rightVal = to_double(right);

        if (b->op == "+") return leftVal + rightVal;
        else if (b->op == "-") return leftVal - rightVal;
        else if (b->op == "*") return leftVal * rightVal;
        else if (b->op == "/") return leftVal / rightVal;
        else if (b->op == "==") return leftVal == rightVal;
        else if (b->op == "!=") return leftVal != rightVal;
        else throw std::runtime_error("Unknown operator: " + b->op);
    }

    throw std::runtime_error("Invalid expression");
}





/*#include "interpreter.h"
#include <stdexcept>
#include <iostream>
#include <variant>
#include <cstring>
#include <string>

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& program) {
    for (const auto& stmt : program) {
        execute(stmt.get());
    }
}

/*void Interpreter::execute(const Stmt* stmt) {
    if (auto printStmt = dynamic_cast<const PrintStmt*>(stmt)) {
        double val = evaluate(printStmt->expr.get());
        std::cout << val << std::endl;
    }
    else if (auto assignStmt = dynamic_cast<const AssignStmt*>(stmt)) {
        double val = evaluate(assignStmt->value.get());
        variables[assignStmt->name] = val; // stores as double, can extend to handle types
    }
    else if (auto declStmt = dynamic_cast<const DeclStmt*>(stmt)) {
        double val = evaluate(declStmt->value.get());
        if (declStmt->varType == TokenType::IntType) {
            variables[declStmt->name] = static_cast<int>(val);
        }
        else if (declStmt->varType == TokenType::FloatType) {
            variables[declStmt->name] = val;
        }
        else {
            throw std::runtime_error("Unknown variable type");
        }
    }
    else if (auto declStmt = dynamic_cast<const DeclStmt*>(stmt)) {
    if (declStmt->varType == TokenType::IntType) {
        variables[declStmt->name] = static_cast<int>(evaluate(declStmt->value.get()));
    }
    else if (declStmt->varType == TokenType::FloatType) {
        variables[declStmt->name] = evaluate(declStmt->value.get());
    }
    else if (declStmt->varType == TokenType::StringType) {
        if (auto strExpr = dynamic_cast<const StringExpr*>(declStmt->value.get())) {
            variables[declStmt->name] = strExpr->value;
        }
        else {
            throw std::runtime_error("Expected string literal for string variable");
        }
        }
        else {
            throw std::runtime_error("Unknown variable type");
        }
    }
    else {
        throw std::runtime_error("Unknown statement type in execute");
    }
}

void Interpreter::execute(const Stmt* stmt) {
    if (auto printStmt = dynamic_cast<const PrintStmt*>(stmt)) {
        auto val = evaluate(printStmt->expr.get());
        // Print variant value:
        std::visit([](auto&& arg) {
            std::cout << arg << std::endl;
        }, val);
    }
    else if (auto assignStmt = dynamic_cast<const AssignStmt*>(stmt)) {
        auto val = evaluate(assignStmt->value.get());
        variables[assignStmt->name] = val;
    }
    else if (auto declStmt = dynamic_cast<const DeclStmt*>(stmt)) {
        auto val = evaluate(declStmt->value.get());

        if (declStmt->varType == TokenType::IntType) {
            // Convert val to int if possible
            if (auto pInt = std::get_if<int>(&val)) {
                variables[declStmt->name] = *pInt;
            }
            else if (auto pDouble = std::get_if<double>(&val)) {
                variables[declStmt->name] = static_cast<int>(*pDouble);
            }
            else {
                throw std::runtime_error("Type mismatch assigning to int variable");
            }
        }
        else if (declStmt->varType == TokenType::FloatType) {
            // Convert val to double if possible
            if (auto pInt = std::get_if<int>(&val)) {
                variables[declStmt->name] = static_cast<double>(*pInt);
            }
            else if (auto pDouble = std::get_if<double>(&val)) {
                variables[declStmt->name] = *pDouble;
            }
            else {
                throw std::runtime_error("Type mismatch assigning to float variable");
            }
        }
        else if (declStmt->varType == TokenType::StringType) {
            // Convert val to string if possible
            if (auto pStr = std::get_if<std::string>(&val)) {
                variables[declStmt->name] = *pStr;
            }
            else {
                throw std::runtime_error("Type mismatch assigning to string variable");
            }
        }
        else {
            throw std::runtime_error("Unknown variable type");
        }
    }
    else {
        throw std::runtime_error("Unknown statement type in execute");
    }
}

std::variant<int, double, std::string> Interpreter::evaluate(const Expr* expr) {
    if (auto n = dynamic_cast<const NumberExpr*>(expr)) {
        // You can decide if NumberExpr stores int or double, here let's say double:
        return n->value;
    }
    else if (auto v = dynamic_cast<const VariableExpr*>(expr)) {
        auto it = variables.find(v->name);
        if (it == variables.end()) {
            throw std::runtime_error("Undefined variable: " + v->name);
        }
        return it->second;
    }
    else if (auto b = dynamic_cast<const BinaryExpr*>(expr)) {
        auto left = evaluate(b->left.get());
        auto right = evaluate(b->right.get());

        // Handle only numeric operations (int or double), strings could be extended later
        // We'll use a helper lambda to get doubles from variant (for now)

        /*auto to_double = [](const auto& val) -> double {
            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, int>)
                return static_cast<double>(val);
            else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, double>)
                return val;
            else
                throw std::runtime_error("Cannot use string in arithmetic operation");
        };
        
        auto checkType = [](const auto& val) {
            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, int> || std::is_same_v<std::decay_t<decltype(val)>, double>)
                return true;
            else
                return false;
            };


        auto to_double = [](const auto& val) -> double {
            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, int>)
                return static_cast<double>(val);
            else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, double>)
                return val;
            else
                throw std::runtime_error("Value is not a number");
            

        };
        auto to_string = [](const auto& val) -> std::string {
            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::string>)
                return val;
            else
                throw std::runtime_error("Value is not a string");
            };
            
            if (std::visit(checkType, left) == false && std::visit(checkType,right)==false)
            {
                std::string leftVal = std::visit(to_string, left);
                std::string rightVal = std::visit(to_string, right);
                return leftVal + rightVal;
            }
            else if (std::visit(checkType, left) !=false || std::visit(checkType, right) != false)
                throw std::runtime_error("Cannot use string and number in arithmetic operation");
            
            double leftVal = std::visit(to_double, left);
            double rightVal = std::visit(to_double, right);

            /*switch (b->op) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            case '/': return leftVal / rightVal;
            default:
                throw std::runtime_error(std::string("Unknown operator: ") + b->op);
            }
            if (b->op == "+") return leftVal + rightVal;
            else if (b->op == "-") return leftVal - rightVal;
            else if (b->op == "*") return leftVal * rightVal;
            else if (b->op == "/") return leftVal / rightVal;
            else if (b->op == "==") return leftVal == rightVal;  // handle boolean result appropriately
            else if (b->op == "!=") return leftVal != rightVal;
            else throw std::runtime_error("Unknown operator: " + b->op);

    }
    else if (auto s = dynamic_cast<const StringExpr*>(expr)) {
        return s->value;
    }

    throw std::runtime_error("Invalid expression");
}
    */