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
    // Ny konstruktor utan krav på indata
    User();

    // Konstruktor med möjlighet att ange användarnamn och lösenord direkt
    User(const std::string& username, const std::string& password);

    // ... övrig kod ...

    // Funktioner för att sätta användarnamn och lösenord
    void setUsername(const std::string& newUsername);
    void setPassword(const std::string& newPassword);

    // Funktion för att hämta användarinput
    static std::string getUserInput(const std::string& prompt);
    bool login();
    // Getter-funktioner
    std::string getUsername() const;
    std::string getPassword() const;
    int getId() const;

    // Funktion för att registrera en ny användare
    void registerNewUser();
    
    
};

#endif
