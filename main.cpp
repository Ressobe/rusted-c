#include <iostream>
#include <fstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/interpreter/Interpreter.h"
#include "runtime/values/Values.h"
#include "runtime/environment/Environment.h"

void testLexer() {
    std::string sourceCode = "let x = (42 * 90) - 120 if";
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    for (const Token& token : tokens) {
        std::cout << "Type: " << token.getType() << ", Value: " << token.getValue() << std::endl;
    }
}

void run () {
    std::string input = R"(

func makeAdder (offset) {
   let x = 10;
   let y = 20;

   print(x + y);
}

makeAdder(3);
    
)";
        
        
    Parser parser;
    Environment env;
    env.createGlobalEnv();

    Program program = parser.produceAST(input);

    Interpreter::evaluate(&program, &env);

    // Evaluate the program in the environment

    // Print or process the result as needed
    // std::cout << ...;

}


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

        /*
        std::cout << "{\n";
        printProgram(program, "  ");
        std::cout << "\n}\n";
        */

        Interpreter::evaluate(&program, &env);
    }

}

int main() {
    //run();
    repl();
    return 0;
}
