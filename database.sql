CREATE TABLE source_type (
    id SERIAL PRIMARY KEY,
    type VARCHAR(255) NOT NULL
);

CREATE TABLE code (
    id SERIAL PRIMARY KEY,
    source_type_id INTEGER REFERENCES source_type(id),
    code TEXT NOT NULL
);

CREATE TABLE execution_stat (
    id SERIAL PRIMARY KEY,
    code_id INTEGER REFERENCES code(id),
    status BOOLEAN,
    execution_time INTEGER,
    execution_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    result TEXT,
    memory_usage INTEGER
);

CREATE TABLE error_type (
    id SERIAL PRIMARY KEY,
    type VARCHAR(255) NOT NULL
);

CREATE TABLE error (
    id SERIAL PRIMARY KEY,
    execution_stat_id INTEGER REFERENCES execution_stat(id),
    error_message TEXT,
    error_type_id INTEGER REFERENCES error_type(id)
);
