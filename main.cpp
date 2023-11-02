#include <iostream>
#include <fstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/interpreter/Interpreter.h"
#include "runtime/values/Values.h"
#include "runtime/environment/Environment.h"


void repl() {
    Parser parser;
    Environment env;
    env.createGlobalEnv();

    std::cout << "\nRustedC v0.1" << std::endl;
    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        // Check for "exit" keyword.
        if (input.find("exit") != std::string::npos) {
            break;
        }

        // Produce AST From source code
        Program program = parser.produceAST(input);

        std::cout << "{\n";
        printProgram(program, "  ");
        std::cout << "\n}\n";

        Interpreter::evaluate(&program, &env);
    }
}

void run() {

    std::ifstream inputFile("code.txt");

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

    Parser parser;
    Environment env;
    env.createGlobalEnv();
    Program program = parser.produceAST(fileContent);

    // std::cout << "{\n";
    // printProgram(program, "  ");
    // std::cout << "\n}\n";

    Interpreter::evaluate(&program, &env);
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
    //repl();
    // run();
    testLexer();
    return 0;
}
