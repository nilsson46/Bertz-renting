#include "database.hpp"
#include "user.hpp"
#include "displayMenu.hpp"
#include <iostream>

int main() {
    Database myDatabase("Bertz.db");
    User newUser;
    DisplayMenu menuOptionsLoggedOut({"Login", "Register new user", "Exit"});
    DisplayMenu menuOptionsLoggedIn({"View Profile", "Logout","Delete user", "Exit"});

    DisplayMenu* currentMenu = &menuOptionsLoggedOut;  // Start with the logged-out menu

    int choice;
    bool loggedIn = false;

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
                           // Set the ID for the newUser object
                           newUser.setId(userId);

                           std::cout << "Login successful. Welcome, " << newUser.getUsername() << " with ID " << userId << "!\n";
                           loggedIn = true;
                           currentMenu = &menuOptionsLoggedIn;  // Switch to the logged-in menu
                       } else {
                           std::cout << "Invalid username or password. Login failed.\n";
                       }
                   }
                   break;

            case 2:
                if (!loggedIn) {
                    newUser.setUsername(newUser.getUserInput("Enter your username: "));
                    newUser.setPassword(newUser.getUserInput("Enter your password: "));
                    newUser.registerNewUser();
                } else {
                    std::cout << "Logout successful. Goodbye, " << newUser.getUsername() << "!\n";
                    loggedIn = false;
                    currentMenu = &menuOptionsLoggedOut;  // Switch back to the logged-out menu
                }
                break;

            case 3:
                            if (loggedIn) {
                                std::cout << "Deleting account...\n";
                                if (myDatabase.deleteUser(newUser.getId())) {
                                    std::cout << "Account deleted successfully.\n";
                                    std::cout << newUser.getId();
                                    loggedIn = false;
                                    currentMenu = &menuOptionsLoggedOut;
                                } else {
                                    std::cout << "Failed to delete account.\n";
                                }
                            } else {
                                std::cout << "Exiting program.\n";
                            }
                            break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 3);

    return 0;
}
