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
    void dropTable(const std::string& tableName);
    void resetDatabase();

    void createTables();
    void dropTables();

    void logCodeExecution(const std::string& code, double executionTime, int errorCount);
    void logError(const std::string& code, const std::string& errorMessage);
    void logAdditionalInfo(const std::string& code, const std::string& additionalInfo);

    std::string getCodeExecutionStats(const std::string& code);
    std::string getErrors(const std::string& code);
    void clearData();

    void updateExecutionTime(const std::string& code, double newExecutionTime);
    std::string getAdditionalInfo(const std::string& code);
    std::vector<std::string> getUniqueCodes();
    void deleteEntry(const std::string& table, int id);
};

#endif
