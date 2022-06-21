//
// Created by daniel on 4/21/22.
//

#ifndef CPP_PROJECT_PASSWORD_H
#define CPP_PROJECT_PASSWORD_H


#include <string>

class Password {
private:
    std::string name;
    std::string username;
    std::string password;
public:
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getUsername() const;
    [[nodiscard]] std::string getPassword() const;
    Password(const std::string& t_name, const std::string& t_username, const std::string& t_password);
    void prettyPrint();
};


#endif //CPP_PROJECT_PASSWORD_H
