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
    bool deleteUser(int userId);
    User getUserByUsername(const string& username);
    std::pair<bool, int> validateUserCredentials(const std::string& username, const std::string& password);
    
    bool addCar(const Car& car, const User& owner);
    bool deleteCar(const string& registrationNumber);
    bool updateCar (const Car& car);
    void getAllAvailableCars();
    int getCarOwnerID(const std::string& registrationNumber) const;
    Car getCarDetails(const std::string& registrationNumber);
    
    bool bookCar(const std::string& registrationNumber, int userId, int passengerCount, const std::chrono::system_clock::time_point& bookingDateTime);
    
    bool updateCarBooking(const std::string& registrationNumber, const std::chrono::system_clock::time_point& bookingDateTime);
    
    bool insertBookingRecord(int carId, int userId, int passengerCount, const std::chrono::system_clock::time_point& bookingDateTime);
    
    bool isCarAvailable(const std::string& registrationNumber, int passengerCount);
    sqlite3* getDb() const;
    
private:
    sqlite3* db;
};

#endif /* database_hpp */
