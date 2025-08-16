# Happyscript

Happyscript is a simple, educational programming language implemented in C++. It features basic variable declarations, arithmetic, comparison and logical operators, control flow (`if`, `else`, `while`), and print statements. Happyscript is designed to be easy to read and extend, making it a great project for learning about interpreters and language design.

## Features

- Integer, floating-point, and string variables
- Arithmetic operators: `+`, `-`, `*`, `/`, `%`
- Comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`
- Control flow: `ana`=`if`, `elsa`=`else`, `fun`=`while`
- Print statements `smile`
- Block statements with `{ ... }`

## Installation

### Option 1: Manual Build

1. **Clone the repository:**
   ```sh
   git clone https://github.com/Matei-thecoder/happyscript.git
   cd happyscript
   ```

2. **Build the interpreter:**
   ```sh
   g++ -std=c++17 -o happyscript main.cpp lexer.cpp parser.cpp interpreter.cpp
   ```

   > Make sure you have a C++17 compatible compiler installed.

### Option 2: Using CMake

1. **Clone the repository:**
   ```sh
   git clone https://github.com/Matei-thecoder/happyscript.git
   cd happyscript
   ```

2. **Create a build directory and run CMake:**
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

   This will generate the `happyscript` executable in the `build` directory.

## Usage

1. **Write your Happyscript code in a file, in the same directory with the `happyscript` executable, e.g. `test.happy`:**
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
        smile(x);
    } elsa {
        smile(x);
    }
    x = x - 1;
}
```

## Language Rules

- **Statement Termination:** Every statement must end with a semicolon (`;`).
- **Supported Types:** The language supports `int`, `float`, and `string` types.
- **String Concatenation:** Only two strings can be concatenated at a time using the `+` operator (e.g., `"hello" + "world"` is valid, but `"hello" + 1` is not).
- **Type Safety:** Addition or concatenation between numbers and strings is not allowed; you cannot add an `int` or `float` to a `string` or vice versa.

## Contributing

Pull requests and suggestions are welcome! If you find a bug or want to request a feature, please open an issue.

## License

This project is licensed under the MIT License.
