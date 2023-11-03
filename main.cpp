#include <iostream>
#include <fstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/interpreter/Interpreter.h"
#include "runtime/values/Values.h"
#include "runtime/environment/Environment.h"


// 1. Lexer
// 2. Parser
// 3. Interpreter

void testIf() {
    // Tokenize your code
    std::string sourceCode =  "if (1) { x = 42; } else { x = 0; }";

    Lexer lexer = Lexer(sourceCode);
    Parser parser;
    Environment env;

    // Parse and evaluate the code
    Program program = parser.produceAST(lexer.tokenize());

    Interpreter::evaluate(&program, &env);

    // RuntimeVal* result = env.lookupVar("x");
    // if (result->type == ValueType::NumberValue) {
    //     NumberVal* numberResult = dynamic_cast<NumberVal*>(result);
    //     std::cout << "Value of 'x' is: " << numberResult->value << std::endl;
    // } else {
    //     std::cout << "'x' is not a numeric value." << std::endl;
    // }
}

void repl() {
    Parser parser;
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
        Program program = parser.produceAST(lexer.tokenize());

        Interpreter::evaluate(&program, &env);
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


    // 2.
    Parser parser;
    Program program = parser.produceAST(lexer.tokenize());

    // printProgram(program, "  ");

    Environment env;
    env.createGlobalEnv();

    // 3.
    RuntimeVal*  val = Interpreter::evaluate(&program, &env);

    std::cout << "Result: " << val->toString() << std::endl;
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
    // testLexer();
    // testIf();
    return 0;
}
