//
// Created by daniel on 4/21/22.
//

#ifndef CPP_PROJECT_USER_H
#define CPP_PROJECT_USER_H


#include "Vault.h"

class User {
private:
    bool needed;
    std::shared_ptr<Vault> vault;
public:
    User(std::shared_ptr<Vault> t_vault);
    bool getNeeded() const;
    void setNeeded(const bool t_needed);
    int run(std::shared_ptr<Data> data);
};

#endif //CPP_PROJECT_USER_H
