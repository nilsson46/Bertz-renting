//
//  database.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#include <iostream>
#include <fstream>
#include <sqlite3.h>

#include "database.hpp"

using std::string;
using std::cout;

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}
Database::Database(const string& dbFileName) {
    std::ifstream file(dbFileName);
    bool fileExists = file.good();
    
    int rc;
    if (!fileExists) {
        cout << "Databasfilen '" << dbFileName << "' existerar inte. Skapar en ny fil.\n";
        rc = sqlite3_open_v2(dbFileName.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
        
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Couldnt create/open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
        
        const char* createTableSQL = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, password TEXT NOT NULL);";
        rc = sqlite3_exec(db, createTableSQL, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Couldnt create 'users' table: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            
            return;
        }
    } else {
        rc = sqlite3_open(dbFileName.c_str(), &db);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Couldnt open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
    }
}


bool Database::addUser(const User& user) {
    string checkSql = "SELECT COUNT(*) FROM users WHERE username = '" + user.getUsername() + "';";
    sqlite3_stmt* checkStmt;
    
    int checkResult = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, 0);
    
    if (checkResult != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    int rowCount = 0;
    
    
    while (sqlite3_step(checkStmt) == SQLITE_ROW) {
        rowCount = sqlite3_column_int(checkStmt, 0);
    }
    
    sqlite3_finalize(checkStmt);
    
    
    if (rowCount > 0) {
        fprintf(stderr, "User already exists. Registration failed.\n");
        return false;
    }
    
    
    string insertSql = "INSERT INTO users (username, password) VALUES ('" + user.getUsername() + "', '" + user.getPassword() + "');";
    int rc = sqlite3_exec(db, insertSql.c_str(), 0, 0, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Could not add user: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    return true;
}

sqlite3* Database::getDb() const {
    return db;
}

std::pair<bool, int> Database::validateUserCredentials(const std::string& username, const std::string& password) {
    string sql = "SELECT id FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    
    if (result != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return std::make_pair(false, 0);
    }
    
    // Bind the parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    int userId = 0;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return std::make_pair(userId > 0, userId);
}


bool Database::deleteUser(int userId) {
    std::string deleteSql = "DELETE FROM users WHERE id = ?;";
    sqlite3_stmt* deleteStmt;
    
    int deleteResult = sqlite3_prepare_v2(db, deleteSql.c_str(), -1, &deleteStmt, 0);
    
    if (deleteResult != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_int(deleteStmt, 1, userId);
    
    int rc = sqlite3_step(deleteStmt);
    
    sqlite3_finalize(deleteStmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error deleting user: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    return true;
}

bool Database::addCar(const Car& car) {
    
    string checkSql = "SELECT COUNT(*) FROM cars WHERE registration_number = '" + car.getRegistrationNumber() + "';";
    sqlite3_stmt* checkStmt;
    
    int checkResult = sqlite3_prepare_v2(db, checkSql.c_str(), -1, &checkStmt, 0);
    
    if (checkResult != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    int rowCount = 0;
    
    while (sqlite3_step(checkStmt) == SQLITE_ROW) {
        rowCount = sqlite3_column_int(checkStmt, 0);
    }
    
    sqlite3_finalize(checkStmt);
    
    if (rowCount > 0) {
        fprintf(stderr, "Car with the same registration number already exists. Adding car failed.\n");
        return false;
    }
    
    
    string insertSql = "INSERT INTO cars (passenger_capacity, model, brand, registration_number) VALUES ("
                        + std::to_string(car.getPassengerCapacity()) + ", '"
                        + car.getModel() + "', '"
                        + car.getBrand() + "', '"
                        + car.getRegistrationNumber() + "');";
    
    int rc = sqlite3_exec(db, insertSql.c_str(), 0, 0, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Could not add car: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    return true;
}




