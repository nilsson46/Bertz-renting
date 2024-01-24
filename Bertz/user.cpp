#include "user.hpp"
#include "database.hpp"
#include <iostream>
#include <utility>

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
    std::getline(std::cin, userInput);
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

void User::setUsername(const std::string& newUsername) {
    username = newUsername;
}

void User::setPassword(const std::string& newPassword) {
    password = newPassword;
}

void User::registerNewUser() {
    Database database("Bertz.db");
    
    // Use std::ws to consume whitespace before entering username
    setUsername(getUserInput("Enter your username: "));

    // Use std::ws to consume whitespace before entering password
    setPassword(getUserInput("Enter your password: "));

    // Check if the user already exists in the database
    string checkSql = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_stmt* checkStmt;

    int checkResult = sqlite3_prepare_v2(database.getDb(), checkSql.c_str(), -1, &checkStmt, 0);

    if (checkResult != SQLITE_OK) {
        cerr << "Could not prepare statement: " << sqlite3_errmsg(database.getDb()) << endl;
        cout << "Failed to register user." << endl;
        return;
    }

    // Bind the username parameter using setUsername value
    sqlite3_bind_text(checkStmt, 1, getUsername().c_str(), -1, SQLITE_STATIC);

    int rowCount = 0;

    while (sqlite3_step(checkStmt) == SQLITE_ROW) {
        rowCount = sqlite3_column_int(checkStmt, 0);
    }

    sqlite3_finalize(checkStmt);

    // Om användaren redan finns, returnera false
    if (rowCount > 0) {
        cout << "User already exists. Registration failed." << endl;
    } else {
        // Använd setUsername och setPassword-värdena när du lägger till användaren
        if (database.addUser(*this)) {
            cout << "Registration successful. Welcome, " << getUsername() << endl;
        } else {
            cout << "Failed to add user to the database." << endl;
        }
    }
}

bool User::login() {
    Database database("Bertz.db");

    std::string enteredUsername = getUserInput("Enter your username: ");
    std::string enteredPassword = getUserInput("Enter your password: ");

    // Check if the entered username and password match the database
    if (auto [success, userId] = database.validateUserCredentials(enteredUsername, enteredPassword); success) {
        std::cout << "Login successful. Welcome, " << enteredUsername << " with ID " << userId << std::endl;
        return true;
    } else {
        std::cout << "Invalid username or password. Login failed." << std::endl;
        return false;
    }
}


