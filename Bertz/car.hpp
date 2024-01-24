//
//  car.hpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-24.
//
#pragma once
#ifndef car_hpp
#define car_hpp

#include <stdio.h>
#include <string>

class Car {
private:
    int id;
    int passengerCapacity;
    std::string model;
    std::string brand;
    std::string registrationNumber;

public:
    Car(int passengerCapacity, const std::string& model, const std::string& brand, const std::string& registrationNumber);
    
    
    int getId() const;
    int getPassengerCapacity() const;
    const std::string& getModel() const;
    const std::string& getBrand() const;
    const std::string& getRegistrationNumber() const;
};

#endif /* car_hpp */
