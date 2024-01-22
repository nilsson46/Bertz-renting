//
//  user.cpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#include "user.hpp"

int User::nextId = 1;

User::User(const string& username, const string& password)
: id(nextId++), username(username), password(password){}

string User::getUsername()const {
    return username;
}

string User::getPassword()const {
    return password;
}

int User::getId() const{
    return id; 
}
