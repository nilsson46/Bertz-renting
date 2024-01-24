//
//  car.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-24.
//

#include "car.hpp"

Car::Car(int passengerCapacity, const std::string& model, const std::string& brand, const std::string& registrationNumber)
    : passengerCapacity(passengerCapacity), model(model), brand(brand), registrationNumber(registrationNumber) {
}

int Car::getId() const {
    return id;
}

int Car::getPassengerCapacity() const {
    return passengerCapacity;
}

const std::string& Car::getModel() const {
    return model;
}

const std::string& Car::getBrand() const {
    return brand;
}

const std::string& Car::getRegistrationNumber() const {
    return registrationNumber;
}
