# Kaleidoscope Compiler (C++)

This is a complete toy compiler for the "Kaleidoscope" programming language, written in modern C++ and built using the LLVM compiler infrastructure. 

It was built incrementally to demonstrate fundamental compiler engineering concepts.

## Compiler Pipeline

The project is organized into distinct stages, each responsible for a specific phase of compilation:

1. **Lexical Analysis (`Lexer.h`, `Lexer.cpp`)**: Tokenizes the raw input string into meaningful symbols, stripping away whitespace and comments.
2. **Syntax Analysis (`Parser.h`, `Parser.cpp`, `AST.h`)**: Uses **Recursive Descent Parsing** to build an Abstract Syntax Tree (AST). It handles operator precedence correctly using **Precedence Climbing**.
3. **IR Code Generation (`CodeGen.h`, `CodeGen.cpp`)**: Traverses the AST and emits LLVM Intermediate Representation (IR). It handles control flow (`if/then/else`, `for` loops) using LLVM Basic Blocks and **PHI Nodes** to maintain Static Single Assignment (SSA) form.
4. **Optimization**: Uses the modern LLVM `PassBuilder` to run optimization passes (like `InstCombine`, `Reassociate`, `GVN`, and `SimplifyCFG`) that fold constants and eliminate dead code at compile time.

## Project Structure
- `include/` and `src/`: The core compiler library (`Lexer`, `Parser`, `AST`, `CodeGen`).
- `test/`: Unit tests and IR generation tests for the various compiler stages.
- `test/programs/`: Sample programs written in the Kaleidoscope language, including a recursive Fibonacci generator and an ASCII Mandelbrot set renderer.
- `.github/workflows/`: CI pipeline configuration testing the compiler on Linux, macOS, and Windows.

## Building the Project

This project uses CMake. You must have LLVM 15+ installed on your system.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Tests

Once built, you can run the test suite using CTest:
```bash
cd build
ctest --output-on-failure
```
