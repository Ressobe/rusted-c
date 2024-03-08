#include "DatabaseHandler.h"

DatabaseHandler::DatabaseHandler(
    const std::string& hostName,  
    const std::string& dbName,
    const std::string& user,
    const std::string& password 
  ) : hostName(hostName), dbName(dbName), user(user), password(password) {
      connection = pqxx::connection("host=" + hostName + " dbname=" + dbName + " user=" + user + " password=" + password);
      if (!connection.is_open()) {
            connection.close();
            throw std::runtime_error("Failed to connect to database.");
      }
}

DatabaseHandler::~DatabaseHandler() {
  connection.close();
}
