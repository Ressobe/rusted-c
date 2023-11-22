#include <iostream>
#include <fstream>
#include <memory>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/interpreter/Interpreter.h"
#include "runtime/values/Values.h"
#include "runtime/environment/Environment.h"


void repl() {
    Parser parser;
    std::unique_ptr<Program> program;

    Environment env;
    env.createGlobalEnv();


    std::cout << "RustedC v0.1" << std::endl;
    while (true) {
        std::string input;
        std::cout << ">>> ";
        std::getline(std::cin, input);

        // Check for "exit" keyword.
        if (input.find("exit") != std::string::npos) {
            break;
        }

        Lexer lexer = Lexer(input);

        // Produce AST From source code
        program = parser.produceAST(lexer.tokenize());

        RuntimeVal* val =  Interpreter::evaluate(program.get(), &env);
        if (val->type == ValueType::NumberValue || val->type == ValueType::StringValue || val->type == ValueType::BooleanValue)
            std::cout << val->toString() << std::endl;
    }
}

void run() {
    std::ifstream inputFile("examples/fibonacci.rc");

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Create a string to store the file content
    std::string line;
    std::string fileContent;

    // Read and append the contents of the file to the string
    while (std::getline(inputFile, line)) {
        fileContent += line + "\n"; // Append each line with a newline character
    }


 
    inputFile.close();

    Lexer lexer = Lexer(fileContent);

    Parser parser;
    std::unique_ptr<Program> program = parser.produceAST(lexer.tokenize());

    Environment env;
    env.createGlobalEnv();

    RuntimeVal* val =  Interpreter::evaluate(program.get(), &env);
    delete val;
}


int main() {
    // repl();
    run();
    return 0;
}
