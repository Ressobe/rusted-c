#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/environment/Environment.h"
#include "runtime/interpreter/Interpreter.h"
#include "database/DatabaseHandler.h"
#include "runtime/values/Values.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ostream>
#include <chrono>
#include <ctime>
#include <string>
#include <fstream>


const char* hostName = "127.0.0.1";
const char* dbName = "rusted-c";
const char* user = "relow";
const char* password = "";


double process_mem_usage() {
    double vm_usage = 0.0;

    unsigned long vsize;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize;
    }

    vm_usage = vsize / 1024.0;

    return vm_usage;
}

void run(std::string code, DatabaseHandler* db, std::string type) {
  std::string errorMessage = "";
  std::string errorType = "";

  auto start = std::chrono::high_resolution_clock::now();
  double mem_before = process_mem_usage();

  RuntimeVal* result = nullptr;

  try {
    Lexer lexer = Lexer(code);
    Parser parser;

    std::unique_ptr<Program> program = parser.produceAST(lexer.getTokens());

    Environment *env = new Environment();

    result = Interpreter::evaluate(program.get(), env);

  } catch(const std::exception& e) {
      std::cerr << e.what() << std::endl;
      if (const LexerError* lexErr = dynamic_cast<const LexerError*>(&e)) {
          errorMessage = lexErr->what();
          errorType = "LEXER";
      } else if (const ParserError* parseErr = dynamic_cast<const ParserError*>(&e)) {
          errorMessage = parseErr->what();
          errorType = "PARSER";
      } else if (const InterpreterError* interpErr = dynamic_cast<const InterpreterError*>(&e)) {
          errorMessage = interpErr->what();
          errorType = "INTERPRETER";
      } else {
          errorMessage = e.what();
          errorType = "UNKNOWN";
      }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto mem_after = process_mem_usage();
  
  if (db != nullptr) {
    auto execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000;
    auto memory_usage = mem_after - mem_before;
    db->addNewStatistic(execution_time, memory_usage, errorMessage, errorType, type, code, result);
  }

  delete result;
}


void repl(DatabaseHandler* db) {
  std::string type = "REPL";
  Parser parser;
  std::unique_ptr<Program> program;

  RuntimeVal *val = nullptr;
  Environment env;

  std::cout << "RustedC v0.1" << std::endl;
  while (true) {
    std::string errorMessage = "";
    std::string errorType = "";

    auto start = std::chrono::high_resolution_clock::now();
    double mem_before = process_mem_usage();


    std::string input;
    std::cout << ">>> ";
    std::getline(std::cin, input);

    if (input.find("exit") != std::string::npos) {
      break;
    }

  try {
    Lexer lexer = Lexer(input);

    program = parser.produceAST(lexer.getTokens());

    val = Interpreter::evaluate(program.get(), &env);
    std::cout << val->toString() << std::endl;
  } catch(const std::exception& e) {
      std::cerr << e.what() << std::endl;
      if (const LexerError* lexErr = dynamic_cast<const LexerError*>(&e)) {
          errorMessage = lexErr->what();
          errorType = "LEXER";
      } else if (const ParserError* parseErr = dynamic_cast<const ParserError*>(&e)) {
          errorMessage = parseErr->what();
          errorType = "PARSER";
      } else if (const InterpreterError* interpErr = dynamic_cast<const InterpreterError*>(&e)) {
          errorMessage = interpErr->what();
          errorType = "INTERPRETER";
      } else {
          errorMessage = e.what();
          errorType = "UNKNOWN";
      }
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto mem_after = process_mem_usage();
  
  if (db != nullptr) {
    auto execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000;
    auto memory_usage = mem_after - mem_before;
    db->addNewStatistic(execution_time, memory_usage, errorMessage, errorType, type, input, val);
  }

  }
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
  DatabaseHandler* database = nullptr;

  try {
     database = new DatabaseHandler(hostName, dbName, user, password);
  } catch (std::runtime_error& e) {
    std::cout << e.what() << std::endl;
    std::cout << "Something wrong with database but you can run code" << std::endl;
  }

  if (argc == 1) {
    repl(database);
  }

  if (argc == 2) {
    if (strcmp(argv[1], "database") == 0) {
      database->displayMenu();
    }
    else {
      run(read_file(argv[1]), database, "FILE");
    }
  }

  delete database;
  return 0;
}
