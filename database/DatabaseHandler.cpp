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

void DatabaseHandler::createTables() {
    // Tworzenie tabeli source_type
    createTable("source_type", "id SERIAL PRIMARY KEY, type VARCHAR(255) NOT NULL");

    // Tworzenie tabeli code
    createTable("code", " id SERIAL PRIMARY KEY, source_type_id INTEGER REFERENCES source_type(id), code TEXT NOT NULL");

    // Tworzenie tabeli execution_stat
    createTable("execution_stat", " id SERIAL PRIMARY KEY, code_id INTEGER REFERENCES code(id), status BOOLEAN, execution_time INTEGER, execution_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP, result TEXT, memory_usage INTEGER");

    // Tworzenie tabeli error_type
    createTable("error_type", "id SERIAL PRIMARY KEY, type VARCHAR(255) NOT NULL");

    // Tworzenie tabeli error
    createTable("error", "id SERIAL PRIMARY KEY, execution_stat_id INTEGER REFERENCES execution_stat(id), error_message TEXT, error_type_id INTEGER REFERENCES error_type(id)");
}

int DatabaseHandler::insertSourceType(const std::string& type) {
    pqxx::work txn(connection);
    pqxx::result res = txn.exec_params("SELECT id FROM source_type WHERE type = $1", type);

    if (!res.empty()) {
        txn.commit();
        return res[0][0].as<int>();
    } else {
        res = txn.exec_params("INSERT INTO source_type (type) VALUES ($1) RETURNING id", type);

        int typeId = res[0][0].as<int>();
        txn.commit();
        return typeId;
  }
}

int DatabaseHandler::insertCode(std::string code, int sourceTypeId) {
    try {
        pqxx::work txn(connection);
        std::string query = "INSERT INTO code (source_type_id, code) VALUES ($1, $2) RETURNING id";
        pqxx::result res = txn.exec_params(query, sourceTypeId, code);
        txn.commit();
        if (!res.empty()) {
            int codeId = res[0][0].as<int>();
            return codeId;
        }
    } catch (const std::exception &e) {
        std::cerr << "Failed to insert code: " << e.what() << std::endl;
    }
    return -1;
}

int DatabaseHandler::insertExecutionStat(
  int codeId, 
  bool status, 
  int executionTime, 
  std::string result, 
  double memoryUsage
  ) {
  try {
      pqxx::work txn(connection);

      std::string query = "INSERT INTO execution_stat (code_id, status, execution_time, result, memory_usage) VALUES ($1, $2, $3, $4, $5) RETURNING id";

      pqxx::result res = txn.exec_params(query, codeId, status, executionTime, result, memoryUsage);

      int statId = res[0][0].as<int>();
      txn.commit();
      return statId;
    } catch (const std::exception& e) {
      std::cerr << "Error while inserting execution stat: " << e.what() << std::endl;
      return -1;
    }

}

int DatabaseHandler::insertErrorType(const std::string& type) {
    try {
        pqxx::work txn(connection);

        std::string checkQuery = "SELECT id FROM error_type WHERE type = $1";
        pqxx::result checkRes = txn.exec_params(checkQuery, type);

        if (!checkRes.empty()) {
            int typeId = checkRes[0][0].as<int>();
            return typeId;
        } else {

            std::string insertQuery = "INSERT INTO error_type (type) VALUES ($1) RETURNING id";
            pqxx::result insertRes = txn.exec_params(insertQuery, type);

            int typeId = insertRes[0][0].as<int>();
            txn.commit();
            return typeId;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error while inserting error type: " << e.what() << std::endl;
        return -1;
    }
}


int DatabaseHandler::insertError(int executionStatId, const std::string& errorMessage, int errorTypeId) {
    try {
        pqxx::work txn(connection);

        std::string insertQuery = "INSERT INTO error (execution_stat_id, error_message, error_type_id) VALUES ($1, $2, $3) RETURNING id";
        pqxx::result insertRes = txn.exec_params(insertQuery, executionStatId, errorMessage, errorTypeId);

        int errorId = insertRes[0][0].as<int>();
        txn.commit();
        return errorId;
    } catch (const std::exception& e) {
        std::cerr << "Error while inserting error: " << e.what() << std::endl;
        return -1;
    }
}
