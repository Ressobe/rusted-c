# Rusted-C 

## Description

This project is an interpreter written in C++ for a simple programming language. 
The interpreter consists of several main modules, including a lexer, parser, abstract syntax tree (AST), and an execution environment. 
The project also includes sample programs written in the interpreted language.

## Language
Checkout [documentation](./docs/DOCS.md) for more information about Rusted-C

## Project Structure

### Directories and Files

- **ast:** Contains the implementation of the abstract syntax tree (AST) in the files `AST.cpp` and `AST.h`.
  
- **examples:** Contains sample programs written in the interpreted language, such as `factorial.rc`, `fibonacci.rc`, `hello_world.rc`, etc.
  
- **lexer:** Contains the implementation of the lexical analyzer in the files `Lexer.cpp` and `Lexer.h`.

- **parser:** Contains the implementation of the parser in the files `Parser.cpp`, `ParserExpr.cpp`, `ParserStmt.cpp`, and `Parser.h`.

- **runtime:** Encompasses the interpreter's execution environment, divided into:
  - **environment:** Contains the implementation of the execution environment in the files `Environment.cpp` and `Environment.h`.
  - **interpreter:** Contains the implementation of the interpreter in the files `Interpreter.cpp`, `InterpreterExpr.cpp`, `InterpreterStmt.cpp`, and `Interpreter.h`.
  - **standard-library:** Contains built-in standard functions in the files `BuiltinFunctions.cpp` and `BuiltinFunctions.h`.
  - **values:** Contains the implementation of values used during interpretation in the files `Values.cpp` and `Values.h`.

- **main.cpp:** The main program file where the `main` function may reside.

- **makefile:** The file for automating the compilation process.


## Compilation and Execution

To compile the project, use the `makefile` by entering the command:

```bash
make
```

Then, run the interpreter on a specific program, for example:

```bash
./bin/rusted-c ./docs/examples/hello_world.rc
```
## Releases

If you don't want to compile project by yourself , you can download binary of interpreter.


## Sample Programs

In the `examples` directory, there are sample programs written in the interpreted language. You can experiment with them to see how the interpreter handles different language constructs.

## Author

- Bartosz Sobina


## License

This project is available under the GNU GENERAL PUBLIC LICENSE. For more information, see the LICENSE file.
