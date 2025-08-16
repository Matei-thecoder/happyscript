#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    std::string source;

    if (argc >= 2) {
        // Read from file
        std::ifstream in(argv[1]);
        if (!in) {
            std::cerr << "Could not open file: " << argv[1] << "\n";
            return 1;
        }
        source.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    } else {
        // Read from stdin
        std::cout << "HappyScript Interpreter! Type your code, finish with Ctrl+D (Linux/mac) or Ctrl+Z (Windows).\n";
        std::string line;
        while (std::getline(std::cin, line)) {
            source += line + "\n";
        }
        if (source.empty()) {
            std::cerr << "No input detected. Exiting.\n";
            return 0;
        }
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
