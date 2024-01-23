//
//  userinput.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#include "userinput.hpp"
#include <iostream>


User UserInputHandler::getUserInput() {
    std::string username, password;

    std::cout << "Ange användarnamn: ";
    std::cin >> username;

    std::cout << "Ange lösenord: ";
    std::cin >> password;

    // Skapa en User med användarinput och returnera den
    return User(username, password);
}
