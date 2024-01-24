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

string getUserInput(const string& prompt) {
    string userInput;
    cout << prompt;
    std::getline(std::cin, userInput);
    return userInput;
}

