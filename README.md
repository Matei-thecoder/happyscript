# Happyscript

Happyscript is a simple, educational programming language implemented in C++. It features basic variable declarations, arithmetic, comparison and logical operators, control flow (`if`, `else`, `while`), and print statements. Happyscript is designed to be easy to read and extend, making it a great project for learning about interpreters and language design.

## Features

- Integer, floating-point, and string variables
- Arithmetic operators: `+`, `-`, `*`, `/`, `%`
- Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Control flow: `ana`, `elsa`, `fun`
- Print statements `smile`
- Block statements with `{ ... }`

## Installation

1. **Clone the repository:**
   ```sh
   git clone https://github.com/yourusername/Happyscript.git
   cd Happyscript/happyscript
   ```

2. **Build the interpreter:**
   ```sh
   g++ -std=c++17 -o happyscript main.cpp lexer.cpp parser.cpp interpreter.cpp
   ```

   > Make sure you have a C++17 compatible compiler installed.

## Usage

1. **Write your Happyscript code in a file, e.g. `test.happy`:**
   ```c
   int x = 5;
   fun (x > 0) {
       smile(x);
       x = x - 1;
   }
   ```

2. **Run the interpreter:**
   ```sh
   ./happyscript test.happy
   ```

   This will execute your Happyscript program and print the output.

## Example

```c
int x = 10;
fun (x >= 0) {
    ana (x % 2 == 0) {
        smile("Even: " + x);
    } elsa {
        smile("Odd: " + x);
    }
    x = x - 1;
}
```

## Contributing

Pull requests and suggestions are welcome! If you find a bug or want to request a feature, please open an issue.

## License

This project is licensed under the MIT License.
