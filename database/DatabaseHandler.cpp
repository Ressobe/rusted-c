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

void DatabaseHandler::createTable(const std::string& tableName, const std::string& tableDefinition) {
    pqxx::work txn(connection);
    txn.exec("CREATE TABLE IF NOT EXISTS " + tableName + " (" + tableDefinition + ")");
    txn.commit();
}

void DatabaseHandler::dropTable(const std::string& tableName) {
    pqxx::work txn(connection);
    txn.exec("DROP TABLE IF EXISTS " + tableName);
    txn.commit();
}

void DatabaseHandler::createTables() {
    // Tworzenie tabeli source_type
    createTable("source_type", "id SERIAL PRIMARY KEY, type VARCHAR(255) NOT NULL");

    // Tworzenie tabeli code
    createTable("code", "id SERIAL PRIMARY KEY, source_type_id INT NOT NULL, code TEXT NOT NULL, FOREIGN KEY (source_type_id) REFERENCES source_type(id)");

    // Tworzenie tabeli status
    createTable("status", "id SERIAL PRIMARY KEY, name VARCHAR(255) NOT NULL");

    // Tworzenie tabeli execution_stat
    createTable("execution_stat", "id SERIAL PRIMARY KEY, code_id INT NOT NULL, status_id INT NOT NULL, execution_time INT, execution_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP, result TEXT, input_data TEXT, output_data TEXT, memory_usage INT, FOREIGN KEY (code_id) REFERENCES code(id), FOREIGN KEY (status_id) REFERENCES status(id)");

    // Tworzenie tabeli error_type
    createTable("error_type", "id SERIAL PRIMARY KEY, type VARCHAR(255) NOT NULL");

    // Tworzenie tabeli error
    createTable("error", "id SERIAL PRIMARY KEY, execution_stat_id INT NOT NULL, error_message TEXT, error_type_id INT NOT NULL, FOREIGN KEY (execution_stat_id) REFERENCES execution_stat(id), FOREIGN KEY (error_type_id) REFERENCES error_type(id)");

}

void DatabaseHandler::dropTables() {
  dropTable("execution_stat");
  dropTable("code");
  dropTable("error");
  dropTable("source_type");
  dropTable("status");
  dropTable("error_type");
}

void DatabaseHandler::resetDatabase() {
    pqxx::work txn(connection);
    txn.exec("DROP DATABASE \"" + dbName + "\"");
    txn.commit();
}
