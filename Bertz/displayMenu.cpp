//
//  displaymenu.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//
#include "displayMenu.hpp"
#include <iostream>

DisplayMenu::DisplayMenu(const std::vector<std::string>& options) : menuOptions(options) {}

void DisplayMenu::showMenu(bool loggedIn) const {
    for (int i = 0; i < menuOptions.size(); i++) {
        std::cout << "[" << (i + 1) << "] " << menuOptions[i] << '\n';
    }
}

int DisplayMenu::getUserChoice() const {
    int choice;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    
    while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
        std::cin >> choice;
    }
    
    return choice;
}
