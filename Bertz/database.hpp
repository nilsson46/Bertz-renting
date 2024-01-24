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
#include "car.hpp"

using std::string;

class Database {
public:
    Database(const string& dbFileName);
    ~Database();
    bool addUser(const User& user);
    bool addCar(const Car& car, const User& owner);
    bool deleteUser(int userId);
    User getUserByUsername(const string& username);
    std::pair<bool, int> validateUserCredentials(const std::string& username, const std::string& password);
    sqlite3* getDb() const;
    
private:
    sqlite3* db;
};




#endif /* database_hpp */
