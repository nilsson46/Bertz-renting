//
//  database.hpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//
#pragma once
#ifndef database_hpp
#define database_hpp

#include <stdio.h>
#include <sqlite3.h>
#include <vector>
#include "user.hpp"

using std::string;

class Database {
public:
    Database(const string& dbFileName);
    bool addUser(const User& user);
    bool deleteUser(const string& username);
    bool authenticateUser(const string& username, const string& password);
    User getUserByUsername(const string& username);
    
private:
    sqlite3* db;
};




#endif /* database_hpp */
