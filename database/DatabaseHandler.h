#ifndef DATABASE_HANDLER
#define DATABASE_HANDLER

#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <vector>
#include <utility>
#include <cstdlib> 
#include "../runtime/values/Values.h"

class DatabaseHandler {
  private:
    std::string hostName;
    std::string dbName;
    std::string user;
    std::string password;
    pqxx::connection connection;

    void createTable(const std::string& tableName, const std::string& tableDefinition);

    std::vector<std::pair<std::string, int>> getTop3ExecutionDays();
    std::vector<std::string> getDistinctSourceTypes();

    int getLongestCodeLength();
    double getAverageExecutionTime();
    int getTotalMemoryUsage();
    int getTotalErrorCount();
    int getSuccessfulExecutionCount();
    int getUnsuccesfulExecutionCount();
    double getAverageCodeLength();
    std::string getMostCommonErrorType();

    void createTables();
    void dropTables();

  public:
    DatabaseHandler(
      const std::string& hostName,  
      const std::string& dbName,
      const std::string& user,
      const std::string& password 
    );

    ~DatabaseHandler();

    int insertSourceType(const std::string& type) ;
    int insertCode(std::string code, int sourceTypeId);
    int insertExecutionStat(int codeId, bool status, int executionTime, std::string result, double memoryUsage);

    int insertErrorType(const std::string& type);
    int insertError(int executionStatId, const std::string& errorMessage, int errorTypeId);

    void addNewStatistic(
      double execution_time,
      double memory_usage, 
      std::string& errorMessage, 
      std::string& errorType, 
      std::string& type,
      std::string& code,
      RuntimeVal* result
    );

    void displayMenu();
};

#endif
