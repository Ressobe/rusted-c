#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/interpreter/Interpreter.h"
#include "runtime/values/Values.h"
#include "runtime/environment/Environment.h"


// TODO
// dodać liczby ujemne
//

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

        if (input.find("exit") != std::string::npos) {
            break;
        }

        Lexer lexer = Lexer(input);

        program = parser.produceAST(lexer.tokenize());

        RuntimeVal* val =  Interpreter::evaluate(program.get(), &env);
        if (val->type == ValueType::NumberValue || val->type == ValueType::StringValue || val->type == ValueType::BooleanValue)
            std::cout << val->toString() << std::endl;
    }
}


void run(char* filename) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file. Probably wrong file name" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf(); 
 
    inputFile.close();    

    std::string fileContent = buffer.str();

    Lexer lexer = Lexer(fileContent);

    Parser parser;
    std::unique_ptr<Program> program = parser.produceAST(lexer.tokenize());

    Environment env;
    env.createGlobalEnv();

    RuntimeVal* val =  Interpreter::evaluate(program.get(), &env);
    delete val;
}


int main(int argc, char** argv) {
    if (argc == 1) {
      repl();
    }

    if (argc == 2) {
      std::string filePath = argv[1];    

      std::filesystem::path filePathObject(filePath);    

      if (!std::filesystem::exists(filePath)) {
        std::cout << "Błąd: plik nie istnieje, niepoprawna ścieżka" << std::endl;
        std::exit(1);
      }

      size_t dotPosition = filePath.find_first_of('.');

      if (dotPosition == std::string::npos) {
        std::cout << "Błąd: niepoprawne rozszerzenie podanego pliku" << std::endl;
        std::exit(1);
      }

      std::string fileName = filePath.substr(0, dotPosition);
      std::string fileExtension = filePath.substr(dotPosition + 1);

      if (fileExtension != "rc" ) {
        std::cout << "Błąd: niepoprawne rozszerzenie podanego pliku" << std::endl;
        std::exit(1);
      }


      run(argv[1]);
    }
    
    return 0;
}
