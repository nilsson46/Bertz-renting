//
//  database.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#include <iostream>
#include <fstream>

#include "database.hpp"

using std::string;
using std::cout;
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
    string sql = "INSERT INTO users (username, password) VALUES ('" + user.getUsername() + "', '" + user.getPassword() + "');";
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Couldnt add user: %s\n", sqlite3_errmsg(db));
        
        return false;
    }

    return true;
}


