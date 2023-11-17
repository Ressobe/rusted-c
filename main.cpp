#include <iostream>
#include <fstream>
#include <memory>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/interpreter/Interpreter.h"
#include "runtime/values/Values.h"
#include "runtime/environment/Environment.h"


// 1. Lexer
// 2. Parser
// 3. Interpreter


void repl() {
    Parser parser;
    std::unique_ptr<Program> program;

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
    }
}

void run() {
    std::ifstream inputFile("./examples/code.rc");

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    // Create a string to store the file content
    std::string fileContent;
    std::string line;

    // Read and append the contents of the file to the string
    while (std::getline(inputFile, line)) {
        fileContent += line + "\n"; // Append each line with a newline character
    }


    // Close the file
    inputFile.close();

    // 1.
    Lexer lexer = Lexer(fileContent);

    // lexer.printTokens();


    // 2.
    Parser parser;
    std::unique_ptr<Program> program = parser.produceAST(lexer.tokenize());

    // printProgram(std::move(program), "  ");

    Environment env;
    env.createGlobalEnv();

    // 3.
    std::unique_ptr<RuntimeVal> val =  Interpreter::evaluate(std::move(program), &env);
    //
    // std::cout << "Result: " << val->toString() << std::endl;
}

void testLexer() {
    std::string testString = "null 100 x \"hello world\" ";

    testString += "3.14";
    //
    // // Keywords
    testString += " let const func if else while return";
    //
    // // Comparison operators
    testString += " == != < <= > >=";
    //
    // // Logical operators
    testString += " && || !";
    //
    // // Binary operators
    testString += " + - * / %";
    //
    // // Assignment operator
    testString += " =";
    //
    // // Punctuation
    testString += " ; ( ) { } [ ] ,";

    Lexer tokenizer = Lexer(testString);

    for (Token& token: tokenizer.tokenize()) {
      std::cout << token.getValue() << " - " <<  token.getTokeTypeName() << std::endl;
    }
}

int main() {
    // repl();
    run();
    return 0;
}
