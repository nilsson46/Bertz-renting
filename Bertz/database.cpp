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

int Database::getCarOwnerID(const std::string& registrationNumber) const {
    int ownerID = -1;
    
    std::string sql = "SELECT owner_id FROM cars WHERE registration_number = ?;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    
    if (result != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return ownerID;
    }
    
    sqlite3_bind_text(stmt, 1, registrationNumber.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        
        ownerID = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    
    return ownerID;
}

Car Database::getCarDetails(const std::string& registrationNumber) {
    Car car;
    
    std::string sql = "SELECT passenger_capacity, model, brand FROM cars WHERE registration_number = ?;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    
    if (result != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return car;
    }
    
    sqlite3_bind_text(stmt, 1, registrationNumber.c_str(), -1, SQLITE_STATIC);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int passengerCapacity = sqlite3_column_int(stmt, 0);
        std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string brand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        
        car = Car(passengerCapacity, model, brand, registrationNumber);
    }
    
    sqlite3_finalize(stmt);
    
    return car;
}

bool Database::addCar(const Car& car, const User& owner) {
    
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
    
    string insertSql = "INSERT INTO cars (owner_id, passenger_capacity, model, brand, registration_number) VALUES ("
    + std::to_string(owner.getId()) + ", "
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

void Database::getAllAvailableCars() {
    std::string sql = "SELECT passenger_capacity, model, brand, registration_number FROM cars WHERE booked_until IS NULL;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    
    if (result != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int passengerCapacity = sqlite3_column_int(stmt, 0);
        std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string brand = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string registrationNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        
        std::cout << "Registration Number: " << registrationNumber << "\n";
        std::cout << "Passenger Capacity: " << passengerCapacity << "\n";
        std::cout << "Model: " << model << "\n";
        std::cout << "Brand: " << brand << "\n";
        std::cout << "------------------------\n";
    }
    
    sqlite3_finalize(stmt);
}


bool Database::deleteCar(const std::string& registrationNumber) {
    std::string deleteSql = "DELETE FROM cars WHERE registration_number = ?;";
    sqlite3_stmt* deleteStmt;
    
    int deleteResult = sqlite3_prepare_v2(db, deleteSql.c_str(), -1, &deleteStmt, 0);
    
    if (deleteResult != SQLITE_OK) {
        fprintf(stderr, "Could not prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_text(deleteStmt, 1, registrationNumber.c_str(), -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(deleteStmt);
    
    sqlite3_finalize(deleteStmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error deleting car: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    return true;
}
bool Database::updateCar(const Car& car) {
    std::string updateSql = "UPDATE cars SET passenger_capacity = ?, model = ?, brand = ? WHERE registration_number = ?;";
    
    sqlite3_stmt* updateStmt;
    int result = sqlite3_prepare_v2(db, updateSql.c_str(), -1, &updateStmt, 0);
    
    if (result != SQLITE_OK) {
        fprintf(stderr, "Could not prepare update statement: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    sqlite3_bind_int(updateStmt, 1, car.getPassengerCapacity());
    sqlite3_bind_text(updateStmt, 2, car.getModel().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(updateStmt, 3, car.getBrand().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(updateStmt, 4, car.getRegistrationNumber().c_str(), -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(updateStmt);
    
    sqlite3_finalize(updateStmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Car update failed: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    return true;
}

bool Database::isCarAvailable(const std::string& registrationNumber, int passengerCount) {
    std::string querySql = "SELECT passenger_capacity FROM cars WHERE registration_number = ? AND booked_until IS NULL;";
    sqlite3_stmt* queryStmt;
    
    int queryResult = sqlite3_prepare_v2(db, querySql.c_str(), -1, &queryStmt, 0);
    if (queryResult != SQLITE_OK) {
        std::cerr << "Could not prepare query statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(queryStmt, 1, registrationNumber.c_str(), -1, SQLITE_STATIC);
    
    int passengerCapacity = -1;
    
    if (sqlite3_step(queryStmt) == SQLITE_ROW) {
        passengerCapacity = sqlite3_column_int(queryStmt, 0);
    }
    
    sqlite3_finalize(queryStmt);
    
    // Check if the car is available and if the passenger count is within capacity
    if (passengerCapacity >= 0 && passengerCapacity >= passengerCount) {
        return true;
    }
    
    return false;
}

bool Database::bookCar(const std::string& registrationNumber, int userId, int passengerCount, const std::chrono::system_clock::time_point& bookingDateTime) {
    if (!isCarAvailable(registrationNumber, passengerCount)) {
        std::cout << "Car with registration number " << registrationNumber << " is not available for booking.\n";
        return false;
    }
    
    // Update the car's booked_until field
    if (!updateCarBooking(registrationNumber, bookingDateTime)) {
        return false;
    }
    
    // Retrieve car details after update
    Car bookedCar = getCarDetails(registrationNumber);
    
    // Check passenger capacity
    if (passengerCount > bookedCar.getPassengerCapacity()) {
        std::cout << "Passenger count exceeds the car's capacity.\n";
        return false;
    }
    
    // Insert booking record
    if (!insertBookingRecord(bookedCar.getId(), userId, passengerCount, bookingDateTime)) {
        return false;
    }
    
    std::cout << "Car with registration number " << registrationNumber << " booked successfully.\n";
    return true;
}

bool Database::updateCarBooking(const std::string& registrationNumber, const std::chrono::system_clock::time_point& bookingDateTime) {
    std::string updateSql = "UPDATE cars SET booked_until = ? WHERE registration_number = ?;";
    sqlite3_stmt* updateStmt;
    
    int updateResult = sqlite3_prepare_v2(db, updateSql.c_str(), -1, &updateStmt, 0);
    if (updateResult != SQLITE_OK) {
        std::cerr << "Could not prepare update statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    std::time_t bookingTime = std::chrono::system_clock::to_time_t(bookingDateTime);
    std::string bookingTimeStr = std::ctime(&bookingTime);
    
    sqlite3_bind_text(updateStmt, 1, bookingTimeStr.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(updateStmt, 2, registrationNumber.c_str(), -1, SQLITE_STATIC);
    
    int executeResult = sqlite3_step(updateStmt);
    if (executeResult != SQLITE_DONE) {
        std::cerr << "Error executing update statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(updateStmt);
        return false;
    }
    
    sqlite3_finalize(updateStmt);
    return true;
}

bool Database::insertBookingRecord(int carId, int userId, int passengerCount, const std::chrono::system_clock::time_point& bookingDateTime) {
    std::string insertBookingSql = "INSERT INTO bookings (car_id, user_id, passenger_count, booking_datetime) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* insertBookingStmt;
    
    int insertResult = sqlite3_prepare_v2(db, insertBookingSql.c_str(), -1, &insertBookingStmt, 0);
    if (insertResult != SQLITE_OK) {
        std::cerr << "Could not prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(insertBookingStmt, 1, carId);
    sqlite3_bind_int(insertBookingStmt, 2, userId);
    sqlite3_bind_int(insertBookingStmt, 3, passengerCount);
    
    std::time_t bookingTimeForInsert = std::chrono::system_clock::to_time_t(bookingDateTime);
    std::string bookingTimeStrForInsert = std::ctime(&bookingTimeForInsert);
    sqlite3_bind_text(insertBookingStmt, 4, bookingTimeStrForInsert.c_str(), -1, SQLITE_STATIC);
    
    int executeResult = sqlite3_step(insertBookingStmt);
    if (executeResult != SQLITE_DONE) {
        std::cerr << "Error executing insert statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(insertBookingStmt);
        return false;
    }
    
    sqlite3_finalize(insertBookingStmt);
    return true;
}

