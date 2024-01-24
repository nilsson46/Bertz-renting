//
//  userinput.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#include "userinput.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::string;


/*User UserInputHandler::getUserInput() {
    std::string username, password;

    std::cout << "Ange användarnamn: ";
    std::cin >> username;

    std::cout << "Ange lösenord: ";
    std::cin >> password;

    // Skapa en User med användarinput och returnera den
    return User(username, password);
} */


string getUserInput(const string& prompt) {
    string userInput;
    cout << prompt;
    std::getline(std::cin, userInput);
    return userInput;
}

