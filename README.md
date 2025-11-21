# SimpleLang

SimpleLang is a tiny expression language that written while learning more about compilers. It uses straightforward C++ and stays small with just a lexer, parser/AST, optimizer, bytecode generator, and a compact virtual machine for scripts with `let`, arithmetic, and `print`.

## Language features

- `let` statements for immutable bindings.
- Numeric literals (integers or decimals) and identifiers.
- Arithmetic operators: `+`, `-`, `*`, `/` with standard precedence and parentheses.
- `print` statements to ofcourse print values.

## Pipeline overview

1. **Lexer** – Converts source text into a token stream.
2. **Parser & AST** – Builds a typed tree of statements/expressions.
3. **Optimizer** – Performs constant folding and strength reduction (`x * 2^k → x << k`).
4. **Bytecode generator** – Translates the optimized AST into a stack-based instruction set.
5. **Virtual machine** – Executes bytecode with a simple stack and variable store.

## Setup

Install the MinGW w64 distribution that provides `g++`. The installer available at https://www.mingw-w64.org/ allows choosing an installation directory such as `D:\mingw`. After installation, add the `bin` directory to the `PATH` environment variable so that `g++.exe` is visible in every shell.

Install Ninja from https://ninja-build.org/ by downloading the Windows archive, extracting `ninja.exe`, and placing it in a directory referenced by `PATH` (for example `D:\Tools`). Verify availability by running `ninja --version` and `g++ --version` in a new PowerShell session.

## Build and execution

Run the following commands from the project root after the toolchain is on `PATH`:

```powershell
cmake -S . -B build -G "Ninja" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build build
build\simplelang.exe examples\sample.ml
```

The final command loads `examples\sample.ml` generates bytecode, and executes the result in the embedded virtual machine. 

Example program (`examples/sample.ml`):

```
let a = 5;
let b = a * 8 + 3;
print b;
```

Output:

```
43
```

## Project layout

```
CMakeLists.txt          # Build definition
src/
  token.h               # Token definitions
  lexer.*               # Hand-written lexer
  ast.h                 # AST nodes
  parser.*              # Recursive descent parser
  optimizer.*           # AST optimization passes
  bytecode.h            # Instruction definitions
  codegen.*             # AST → bytecode compiler
  vm.*                  # Stack-based virtual machine
  main.cpp              # CLI driver
examples/sample.ml      # Demonstration program
```

## Language syntax reference

Programs are sequences of statements terminated by semicolons. The supported statements are as follows.

`let name = expression;` introduces a variable initialized with the value of `expression`.

`print expression;` evaluates the expression and writes the value on its own line.

Expressions support the following constructs:

`number` where the numeric literal can be integer or decimal.

`identifier` where the identifier must have been initialized by a `let` statement.

`(expression)` to override precedence.

`expression + expression`, `expression - expression`, `expression * expression`, `expression / expression`.

`-expression` and `+expression` as unary forms.

The parser enforces the usual precedence levels (multiplication and division bind more tightly than addition and subtraction) and associativity is left to right. Division by zero at runtime raises an error. Identifiers that have not been introduced by `let` also raise runtime errors. The optimizer operates only on numeric literals, so semantic behavior remains obvious.

### Update: 22 Nov 2025

Pushed missing CMakeList

## Deployment

Deployed the project on GCP. Now you can try it live. Basically hosted a simple server that takes input in the browser and saves as a temporary .ml file and runs the build simplelang to run and print on the browser.
