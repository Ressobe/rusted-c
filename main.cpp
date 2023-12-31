#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/environment/Environment.h"
#include "runtime/interpreter/Interpreter.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

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

    program = parser.produceAST(lexer.getTokens());

    RuntimeVal *val = Interpreter::evaluate(program.get(), &env);
    std::cout << val->toString() << std::endl;
  }
}

void run(std::string fileContent) {
  Lexer lexer = Lexer(fileContent);
  Parser parser;
    
  std::unique_ptr<Program> program = parser.produceAST(lexer.getTokens());

  Environment *env = new Environment();
  env->createGlobalEnv();

  RuntimeVal *val = Interpreter::evaluate(program.get(), env);
  delete val;
  delete env;
}

std::string read_file(std::string filePath) {
  std::filesystem::path filePathObject(filePath);

  if (!std::filesystem::exists(filePath)) {
    std::cout << "Error: this file does not exist" << std::endl;
    std::exit(1);
  }

  size_t dotPosition = filePath.find_last_of('.');

  if (dotPosition == std::string::npos) {
    std::cout << "Error: incorret file extension" << std::endl;
    std::exit(1);
  }

  std::string fileName = filePath.substr(0, dotPosition);
  std::string fileExtension = filePath.substr(dotPosition + 1);

  if (fileExtension != "rc") {
    std::cout << "Error: incorret file extension" << std::endl;
    std::exit(1);
  }

  std::ifstream inputFile(filePath);

  if (!inputFile.is_open()) {
    std::cerr << "Failed to open the file. Probably wrong file name"
              << std::endl;
    std::exit(1);
  }

  std::stringstream buffer;
  buffer << inputFile.rdbuf();

  inputFile.close();
  return buffer.str();
}


int main(int argc, char **argv) {
  if (argc == 1) {
    repl();
  }

  if (argc == 2) {
    run(read_file(argv[1]));
  }
  return 0;
}
