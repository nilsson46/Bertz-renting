//
//  user.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#include "user.hpp"

User::User(const string& username, const string& password)
: username(username), password(password){}

string User::getUsername()const {
    return username;
}

string User::getPassword()const {
    return password;
}
