//
// Created by daniel on 4/21/22.
//

#include <iostream>
#include "Password.h"

#define BOLD "\x1B[1m"
#define UNDERLINED "\x1B[4m"
#define RESET "\033[0m"

std::string Password::getName() const {
    return this->name;
}

std::string Password::getUsername() const {
    return this->username;
}

std::string Password::getPassword() const {
    return this->password;
}

Password::Password(const std::string &t_name, const std::string &t_username, const std::string &t_password) {
    this->name = t_name;
    this->username = t_username;
    this->password = t_password;
}

void Password::prettyPrint(){
    std::cout << BOLD << "-> " << this->getName() << RESET << " " << UNDERLINED  << this->getUsername() << RESET << "\n" << this->password ;
}
