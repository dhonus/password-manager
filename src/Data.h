//
// Created by daniel on 4/21/22.
//

#ifndef CPP_PROJECT_DATA_H
#define CPP_PROJECT_DATA_H

#include <vector>
#include <memory>
#include <list>
#include "Password.h"

class Data {
private:
    std::vector<std::shared_ptr<Password>> passwords;
    bool valid = false;
public:
    bool substr(const std::string& A, const std::string& B);
    std::vector<std::shared_ptr<Password>> find(const std::string& t_name);
    int insert(std::shared_ptr<Password> t_password);
    std::shared_ptr<Password> remove(std::shared_ptr<Password> t_password);
    std::vector<std::shared_ptr<Password>> getData();
    bool getValid();
    void setValid(bool t_valid);
};


#endif //CPP_PROJECT_DATA_H
