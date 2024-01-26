//
//  user.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//
#include "user.hpp"
#include "database.hpp"
#include <iostream>
#include <utility>
#include <limits>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::cerr;

int User::nextId = 1;
User::User() : id(nextId++), username(""), password("") {}
User::User(const std::string& username, const std::string& password)
: id(nextId++), username(username), password(password) {}

std::string User::getUserInput(const std::string& prompt) {
    std::string userInput;
    std::cout << prompt;
    cin.sync();
    //std::getline(std::cin, userInput);
    cin >> userInput;
    return userInput;
}
int User::getUserInputInt(const std::string& prompt) {
    int userInput;
    std::cout << prompt;
    cin >> userInput;
    return userInput;
}
std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

int User::getId() const {
    return id;
}
void User::setId(int id) {
    this->id = id;
}
void User::setUsername(const std::string& newUsername) {
    username = newUsername;
}

void User::setPassword(const std::string& newPassword) {
    password = newPassword;
}

bool User::registerNewUser() {
    Database database("Bertz.db");
    
    setUsername(getUserInput("Enter your username: "));
    setPassword(getUserInput("Enter your password: "));
    
    string checkSql = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_stmt* checkStmt;
    
    int checkResult = sqlite3_prepare_v2(database.getDb(), checkSql.c_str(), -1, &checkStmt, 0);
    
    if (checkResult != SQLITE_OK) {
        cerr << "Could not prepare statement: " << sqlite3_errmsg(database.getDb()) << endl;
        cout << "Failed to register user." << endl;
        return false;
    }
    
    sqlite3_bind_text(checkStmt, 1, getUsername().c_str(), -1, SQLITE_STATIC);
    
    int rowCount = 0;
    
    while (sqlite3_step(checkStmt) == SQLITE_ROW) {
        rowCount = sqlite3_column_int(checkStmt, 0);
    }
    
    sqlite3_finalize(checkStmt);
    
    if (rowCount > 0) {
        cout << "User already exists. Registration failed." << endl;
        return false;  // User already exists, return false
    }
    
    // User doesn't exist, proceed to add user
    string insertSql = "INSERT INTO users (username, password) VALUES ('" + getUsername() + "', '" + getPassword() + "');";
    int rc = sqlite3_exec(database.getDb(), insertSql.c_str(), 0, 0, 0);
    
    if (rc != SQLITE_OK) {
        cerr << "Could not add user: " << sqlite3_errmsg(database.getDb()) << endl;
        return false;  // Failed to add user, return false
    }
    
    cout << "Registration successful. Welcome, " << getUsername() << endl;
    return true;
}

bool User::login() {
    Database database("Bertz.db");
    
    std::string enteredUsername = getUserInput("Enter your username: ");
    std::string enteredPassword = getUserInput("Enter your password: ");
    
    if (auto [success, userId] = database.validateUserCredentials(enteredUsername, enteredPassword); success) {
        std::cout << "Login successful. Welcome, " << enteredUsername << " with ID " << userId << std::endl;
        return true;
    } else {
        std::cout << "Invalid username or password. Login failed." << std::endl;
        return false;
    }
}


