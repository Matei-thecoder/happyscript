#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "HappyScript Interpreter! Type your code, finish with Ctrl+D (Linux/mac) or Ctrl+Z (Windows).\n";

    std::string source, line;
    while (std::getline(std::cin, line)) {
        source += line + "\n";
    }

    if (source.empty()) {
        std::cerr << "No input detected. Exiting.\n";
        return 0;
    }

    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto program = parser.parseProgram();

        Interpreter interpreter;
        interpreter.interpret(program);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
