//
//  main.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#include "database.hpp"
#include "user.hpp"
#include "userinput.hpp"
#include <iostream>

int main() {
    // Skapa en databas med namnet "mydatabase.db"
    Database myDatabase("Bertz.db");

    // Hämta användarinput med UserInputHandler
    User newUser = UserInputHandler::getUserInput();

    // Lägg till användaren i databasen
    if (myDatabase.addUser(newUser)) {
        std::cout << "Användare tillagd i databasen framgångsrikt.\n";
    } else {
        std::cerr << "Misslyckades med att lägga till användare i databasen.\n";
    }

    return 0;
}

