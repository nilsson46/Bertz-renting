// user.hpp
#pragma once
#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
private:
    static int nextId;
    
    int id;
    std::string username;
    std::string password;
    
public:

    User();
    

    User(const std::string& username, const std::string& password);

    void setUsername(const std::string& newUsername);
    void setPassword(const std::string& newPassword);
    
    static std::string getUserInput(const std::string& prompt);
    bool login();
  
    std::string getUsername() const;
    std::string getPassword() const;
    int getId() const;
    void setId(int id);

    void registerNewUser();
    
    
};

#endif
