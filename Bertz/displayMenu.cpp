// displayMenu.cpp
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
    std::cin >> choice;
    return choice;
}
