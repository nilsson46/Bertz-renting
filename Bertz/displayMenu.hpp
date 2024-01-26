//
//  displaymenu.hpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//
#pragma once
#ifndef displayMenu_hpp
#define displayMenu_hpp

#include <vector>
#include <string>

class DisplayMenu {
public:
    DisplayMenu(const std::vector<std::string>& options);
    
    void showMenu(bool loggedIn = false) const;
    int getUserChoice() const;
    
private:
    std::vector<std::string> menuOptions;
};

#endif
