//
//  main.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//
#include "database.hpp"
#include "user.hpp"
#include "displayMenu.hpp"
#include "userinput.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
int main() {
    Database myDatabase("Bertz.db");
    //User newUser;
    DisplayMenu menuOptionsLoggedOut({"Login", "Register new user", "Exit"});
    DisplayMenu menuOptionsLoggedIn({"View Profile", "Logout","Delete user", "Add Car" ,"Delete Car", "Show Available Cars" ,"Update existing car", "Book a car"});
    
    DisplayMenu* currentMenu = &menuOptionsLoggedOut;
    User* newUser = nullptr;
    int choice;
    bool loggedIn = false;
    bool exitProgram = false;
    
    do {
        currentMenu->showMenu(loggedIn);
        choice = currentMenu->getUserChoice();
        
        switch (choice) {
            case 1:
                std::cout << (loggedIn ? "Viewing profile...\n" : "Login...\n");
                if (!loggedIn) {
                    newUser = new User();
                    newUser->setUsername(newUser->getUserInput("Enter your username: "));
                    newUser->setPassword(newUser->getUserInput("Enter your password: "));
                    auto [loginSuccess, userId] = myDatabase.validateUserCredentials(newUser->getUsername(), newUser->getPassword());
                    if (loginSuccess) {
                        
                        newUser->setId(userId);
                        
                        std::cout << "Login successful. Welcome, " << newUser->getUsername() << " with ID " << userId << "!\n";
                        loggedIn = true;
                        currentMenu = &menuOptionsLoggedIn;
                    } else {
                        std::cout << "Invalid username or password. Login failed.\n";
                    }
                }
                break;
                
            case 2:
                if (!loggedIn) {
                    newUser->registerNewUser();
                } else {
                    std::cout << "Logout successful. Goodbye, " << newUser->getUsername() << "!\n";
                    loggedIn = false;
                    currentMenu = &menuOptionsLoggedOut;
                }
                break;
                
            case 3:
                if (loggedIn) {
                    std::cout << "Deleting account...\n";
                    if (myDatabase.deleteUser(newUser->getId())) {
                        std::cout << "Account deleted successfully.\n";
                        loggedIn = false;
                        currentMenu = &menuOptionsLoggedOut;
                        delete newUser;
                        newUser = nullptr;
                    } else {
                        std::cout << "Failed to delete account.\n";
                    }
                } else {
                    std::cout << "Exiting program.\n";
                    exitProgram = true;
                    break;
                    
                case 4:
                    if (loggedIn) {
                        std::cout << "Adding a car...\n";
                        
                        
                        int passengerCapacity = newUser->getUserInputInt("Enter passenger capacity: ");
                        std::string model = newUser->getUserInput("Enter car model: ");
                        std::string brand = newUser->getUserInput("Enter car brand: ");
                        std::string registrationNumber = newUser->getUserInput("Enter registration number: ");
                        
                        
                        Car newCar(passengerCapacity, model, brand, registrationNumber);
                        
                        
                        if (myDatabase.addCar(newCar, *newUser)) {
                            std::cout << "Car added successfully.\n";
                        } else {
                            std::cout << "Failed to add car.\n";
                        }
                    } else {
                        std::cout << "Exiting program.\n";
                        exitProgram = true;
                    }
                    break;
                    
                case 5:
                    if (loggedIn) {
                        std::string registrationNumber;
                        std::cout << "Enter the registration number of the car you want to delete: ";
                        std::cin >> registrationNumber;
                        if (myDatabase.deleteCar(registrationNumber)) {
                            std::cout << "Car deleted successfully.\n";
                        } else {
                            std::cout << "Failed to delete car.\n";
                        }
                    } else {
                        std::cout << "Invalid choice. Please try again.\n";
                    }
                    break;
                    
                case 6:
                    if(loggedIn){
                        myDatabase.getAllAvailableCars();
                    }else{
                        std::cout << "Didnt find any available cars.\n";
                    }
                    break;
                    
                case 7:
                    if (loggedIn) {
                        std::string registrationNumberToUpdate = newUser->getUserInput("Enter registration number of the car you want to update: ");
                        int ownerId = myDatabase.getCarOwnerID(registrationNumberToUpdate);
                        
                        if (ownerId == newUser->getId()) {
                            
                            Car existingCar = myDatabase.getCarDetails(registrationNumberToUpdate);
                            
                            std::cout << "Current details for car with registration number " << registrationNumberToUpdate << ":\n";
                            std::cout << "Passenger capacity: " << existingCar.getPassengerCapacity() << "\n";
                            std::cout << "Model: " << existingCar.getModel() << "\n";
                            std::cout << "Brand: " << existingCar.getBrand() << "\n";
                            
                            
                            int newPassengerCapacity = newUser->getUserInputInt("Enter new passenger capacity: ");
                            std::string newModel = newUser->getUserInput("Enter new car model: ");
                            std::string newBrand = newUser->getUserInput("Enter new car brand: ");
                            
                            
                            Car updatedCar(newPassengerCapacity, newModel, newBrand, registrationNumberToUpdate);
                            
                            
                            if (myDatabase.updateCar(updatedCar)) {
                                std::cout << "Car updated successfully.\n";
                            } else {
                                std::cout << "Failed to update car.\n";
                            }
                        } else {
                            std::cout << "You don't have permission to update this car.\n";
                        }
                    }
                    break;
                case 8:
                    if (loggedIn) {
                        
                        std::cout << "Available cars for booking:\n";
                        myDatabase.getAllAvailableCars();
                        
                        int passengerCount = newUser->getUserInputInt("Enter the number of passengers: ");
                        std::string registrationNumberToBook = newUser->getUserInput("Enter the registration number of the car you want to book: ");
                        std::string bookingDateTime = newUser->getUserInput("Enter the booking date and time (YYYY-MM-DD HH:MM): ");
                        
                        std::tm tm = {};
                        std::istringstream ss(bookingDateTime);
                        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
                        
                        if (ss.fail()) {
                            std::cout << "Invalid date and time format. Please use YYYY-MM-DD HH:MM.\n";
                            break;
                        }
                        
                        std::chrono::system_clock::time_point bookingTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                        
                        if (myDatabase.isCarAvailable(registrationNumberToBook, passengerCount)) {
                            if (myDatabase.bookCar(registrationNumberToBook, newUser->getId(), passengerCount, bookingTimePoint)) {
                                std::cout << "Car booked successfully.\n";
                            } else {
                                std::cout << "Failed to book the car.\n";
                            }
                        } else {
                            std::cout << "Selected car is not available or does not meet passenger requirements.\n";
                        }
                    } else {
                        std::cout << "Invalid choice. Please try again.\n";
                    }
                    break;
                    
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                }
                
        }
    }while (!exitProgram);
    
    delete newUser;
    return 0;
}
