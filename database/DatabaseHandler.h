#ifndef DATABASE_HANDLER
#define DATABASE_HANDLER

#include <iostream>
#include <string>
#include <pqxx/pqxx>

class DatabaseHandler {
  private:
    std::string hostName;
    std::string dbName;
    std::string user;
    std::string password;
    pqxx::connection connection;

    
  public:
    DatabaseHandler(
      const std::string& hostName,  
      const std::string& dbName,
      const std::string& user,
      const std::string& password 
    );

    ~DatabaseHandler();

    void createTable(const std::string& tableName, const std::string& tableDefinition);
    void createTables();

    int insertSourceType(const std::string& type) ;
    int insertCode(std::string code, int sourceTypeId);
    int insertExecutionStat(int codeId, bool status, int executionTime, std::string result, double memoryUsage);

    int insertErrorType(const std::string& type);
    int insertError(int executionStatId, const std::string& errorMessage, int errorTypeId);
};

#endif
