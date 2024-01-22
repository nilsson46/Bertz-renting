//
//  user.hpp
//  Bertz
//
//  Created by Simon Nilsson on 2024-01-22.
//

#ifndef user_hpp
#define user_hpp
#pragma once

#include <iostream>
#include <stdio.h>

using std::string;


class User{
public:
    //Constructor
    User(const string& username, const string& password);
    string getUsername() const;
    string getPassword() const;
    int getId() const;
    
private:
    static int nextId;
    int id;
    string username;
    string password;
}; 

#endif /* user_hpp */
