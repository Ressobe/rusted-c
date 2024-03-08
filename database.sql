-- Tworzenie tabeli source_type
CREATE TABLE source_type (
    id SERIAL PRIMARY KEY,
    type VARCHAR(255) NOT NULL
);

-- Tworzenie tabeli code
CREATE TABLE code (
    id SERIAL PRIMARY KEY,
    source_type_id INT NOT NULL,
    code TEXT NOT NULL,
    FOREIGN KEY (source_type_id) REFERENCES source_type(id)
);

-- Tworzenie tabeli status
CREATE TABLE status (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL
);

-- Tworzenie tabeli execution_stat
CREATE TABLE execution_stat (
    id SERIAL PRIMARY KEY,
    code_id INT NOT NULL,
    status_id INT NOT NULL,
    execution_time INT,
    execution_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    result TEXT,
    input_data TEXT,
    output_data TEXT,
    memory_usage INT,
    FOREIGN KEY (code_id) REFERENCES code(id),
    FOREIGN KEY (status_id) REFERENCES status(id)
);

-- Tworzenie tabeli error_type
CREATE TABLE error_type (
    id SERIAL PRIMARY KEY,
    type VARCHAR(255) NOT NULL
);

-- Tworzenie tabeli error
CREATE TABLE error (
    id SERIAL PRIMARY KEY,
    execution_stat_id INT NOT NULL,
    error_message TEXT,
    error_type_id INT NOT NULL,
    FOREIGN KEY (execution_stat_id) REFERENCES execution_stat(id),
    FOREIGN KEY (error_type_id) REFERENCES error_type(id)
);
