#include "database.hpp"
#include "user.hpp"
#include "displayMenu.hpp"
#include "userinput.hpp"
#include <iostream>

int main() {
    Database myDatabase("Bertz.db");
    User newUser;
    DisplayMenu menuOptionsLoggedOut({"Login", "Register new user", "Exit"});
    DisplayMenu menuOptionsLoggedIn({"View Profile", "Logout","Delete user", "Add Car" "Exit"});
    
    DisplayMenu* currentMenu = &menuOptionsLoggedOut;  // Start with the logged-out menu
    
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
                    newUser.setUsername(newUser.getUserInput("Enter your username: "));
                    newUser.setPassword(newUser.getUserInput("Enter your password: "));
                    auto [loginSuccess, userId] = myDatabase.validateUserCredentials(newUser.getUsername(), newUser.getPassword());
                    if (loginSuccess) {
                        
                        newUser.setId(userId);
                        
                        std::cout << "Login successful. Welcome, " << newUser.getUsername() << " with ID " << userId << "!\n";
                        loggedIn = true;
                        currentMenu = &menuOptionsLoggedIn;
                    } else {
                        std::cout << "Invalid username or password. Login failed.\n";
                    }
                }
                break;
                
            case 2:
                if (!loggedIn) {
                    newUser.registerNewUser();
                } else {
                    std::cout << "Logout successful. Goodbye, " << newUser.getUsername() << "!\n";
                    loggedIn = false;
                    currentMenu = &menuOptionsLoggedOut;
                }
                break;
                
            case 3:
                if (loggedIn) {
                    std::cout << "Deleting account...\n";
                    if (myDatabase.deleteUser(newUser.getId())) {
                        std::cout << "Account deleted successfully.\n";
                        loggedIn = false;
                        currentMenu = &menuOptionsLoggedOut;
                        newUser = User();  // Återställ newUser-objektet efter att kontot har tagits bort
                    } else {
                        std::cout << "Failed to delete account.\n";
                    }
                } else {
                    std::cout << "Exiting program.\n";
                    exitProgram = true;  // Sätt flaggan för att indikera att programmet ska avslutas
                }
                break;
                
            case 4:
                if (loggedIn) {
                    std::cout << "Adding a car...\n";
                    
                    // Collect information for the new car
                    int passengerCapacity = newUser.getUserInputInt("Enter passenger capacity: ");
                    std::string model = newUser.getUserInput("Enter car model: ");
                    std::string brand = newUser.getUserInput("Enter car brand: ");
                    std::string registrationNumber = newUser.getUserInput("Enter registration number: ");
                    
                    // Create a Car object with the collected information
                    Car newCar(passengerCapacity, model, brand, registrationNumber);
                    
                    // Add the car to the database
                    if (myDatabase.addCar(newCar, newUser)) {
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
                std::cout << "Exiting program.\n";
                exitProgram = true;
                break;
                
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        
    } while (!exitProgram);
    
    return 0;
}
