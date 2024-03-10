#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/environment/Environment.h"
#include "runtime/interpreter/Interpreter.h"
#include "database/DatabaseHandler.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <ostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>


const char* hostName = "127.0.0.1";
const char* dbName = "rusted-c";
const char* user = "relow";
const char* password = "";


std::string get_current_datetime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


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
    env->createGlobalEnv();

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

  // auto end = std::chrono::high_resolution_clock::now();
  // auto mem_after = process_mem_usage();
  //
  //
  // std::cout << "hi";
  //
  // if (db != nullptr) {
  //   auto execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000;
  //   auto memory_usage = mem_after - mem_before;
  //   bool isSucces = (errorMessage.empty()) ?  true : false;
  //
  //   int typeId = db->insertSourceType(type);
  //   int codeId = db->insertCode(code, typeId);
  //   int executionStat = db->insertExecutionStat(codeId, isSucces,  execution_time,  result->toString(), memory_usage);
  //
  //   if (!isSucces) {
  //     int errorTypeId = db->insertErrorType(errorType);
  //     db->insertError(executionStat, errorMessage, errorTypeId);
  //   }
  //
  // }
  //
  // delete result;
}


void repl(DatabaseHandler* db) {
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
    run(input, db, "REPL");
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
    run(read_file(argv[1]), database, "FILE");
  }

  delete database;
  return 0;
}
