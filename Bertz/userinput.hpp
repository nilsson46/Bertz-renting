//
//  userinput.hpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//
#pragma once
#ifndef userinput_hpp
#define userinput_hpp
#include "user.hpp"

#include <stdio.h>
#include <string>
#include <utility>

class UserInputHandler {
public:
    static User getUserInput();
};
#endif /* userinput_hpp */
