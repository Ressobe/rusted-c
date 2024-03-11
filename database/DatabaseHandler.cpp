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

std::string DatabaseHandler::getMostCommonErrorType() {
  std::string mostCommonErrorType;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT error_type.type, COUNT(error.error_type_id) as error_count "
                                     "FROM error "
                                     "JOIN error_type ON error.error_type_id = error_type.id "
                                     "GROUP BY error_type.type "
                                     "ORDER BY error_count DESC "
                                     "LIMIT 1");
      if (!result.empty()) {
          mostCommonErrorType = result[0]["type"].as<std::string>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting most common error type: " << e.what() << std::endl;
  }

  return mostCommonErrorType;
}


std::vector<std::pair<std::string, int>> DatabaseHandler::getTop3ExecutionDays() {
  std::vector<std::pair<std::string, int>> top3Days;
  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT DATE(execution_date) as execution_day, COUNT(*) as code_count "
                                     "FROM execution_stat "
                                     "GROUP BY execution_day "
                                     "ORDER BY code_count DESC "
                                     "LIMIT 3");

      for (const auto &row : result) {
          std::string executionDay = row["execution_day"].as<std::string>();
          int codeCount = row["code_count"].as<int>();
          top3Days.push_back(std::make_pair(executionDay, codeCount));
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting top 3 execution days: " << e.what() << std::endl;
  }
  return top3Days; 
}

std::vector<std::string> DatabaseHandler::getDistinctSourceTypes() {
  std::vector<std::string> distinctSourceTypes;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT DISTINCT type FROM source_type");

      for (const auto &row : result) {
          distinctSourceTypes.push_back(row["type"].as<std::string>());
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting distinct source types: " << e.what() << std::endl;
  }

  return distinctSourceTypes;
}

int DatabaseHandler::getLongestCodeLength() {
  int longestCodeLength = 0;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT MAX(LENGTH(code)) as max_length FROM code");

      if (!result.empty()) {
          longestCodeLength = result[0]["max_length"].as<int>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting longest code length: " << e.what() << std::endl;
  }

  return longestCodeLength;
}

double DatabaseHandler::getAverageExecutionTime() {
  double averageExecutionTime = 0.0;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT AVG(execution_time) as avg_time FROM execution_stat");

      if (!result.empty()) {
          averageExecutionTime = result[0]["avg_time"].as<double>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting average execution time: " << e.what() << std::endl;
  }

  return averageExecutionTime;
}

int DatabaseHandler::getTotalMemoryUsage() {
  int totalMemoryUsage = 0;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT SUM(memory_usage) as total_usage FROM execution_stat");

      if (!result.empty()) {
          totalMemoryUsage = result[0]["total_usage"].as<int>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting total memory usage: " << e.what() << std::endl;
  }

  return totalMemoryUsage;
}

int DatabaseHandler::getTotalErrorCount() {
  int totalErrorCount = 0;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT COUNT(*) AS error_count FROM error");

      if (!result.empty()) {
          totalErrorCount = result[0]["error_count"].as<int>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting total error count: " << e.what() << std::endl;
  }

  return totalErrorCount;
}

int DatabaseHandler::getSuccessfulExecutionCount() {
  int successfulExecutionCount = 0;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT COUNT(*) AS success_count FROM execution_stat WHERE status = true");

      if (!result.empty()) {
          successfulExecutionCount = result[0]["success_count"].as<int>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting successful execution count: " << e.what() << std::endl;
  }

  return successfulExecutionCount;
}

int DatabaseHandler::getUnsuccesfulExecutionCount() {
  int unsucessfulExecutionCount = 0;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT COUNT(*) AS unsuccess_count FROM execution_stat WHERE status = false");

      if (!result.empty()) {
          unsucessfulExecutionCount = result[0]["unsuccess_count"].as<int>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting unsuccessful execution count: " << e.what() << std::endl;
  }

  return unsucessfulExecutionCount;

}

double DatabaseHandler::getAverageCodeLength() {
  double averageCodeLength = 0.0;

  try {
      pqxx::work txn(connection);
      pqxx::result result = txn.exec("SELECT AVG(LENGTH(code)) AS avg_length FROM code");

      if (!result.empty()) {
          averageCodeLength = result[0]["avg_length"].as<double>();
      }
  } catch (const std::exception &e) {
      std::cerr << "Error getting average code length: " << e.what() << std::endl;
  }

  return averageCodeLength;
}
