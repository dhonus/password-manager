//
// Created by daniel on 4/21/22.
//

#include "Data.h"
#include <cctype>

int Data::insert(std::shared_ptr<Password> t_password) {
    this->passwords.push_back(std::move(t_password));
    return EXIT_SUCCESS;
}

std::shared_ptr<Password> Data::remove(std::shared_ptr<Password> t_password) {
    for (size_t i = 0; i < this->passwords.size(); ++i){
        if (this->passwords[i] == t_password){
            this->passwords.erase(this->passwords.begin() + i);
        }
    }
    return std::shared_ptr<Password>();
}


bool Data::substr(const std::string& A, const std::string& B) {
    size_t b_index = 0;
    for (size_t i = 0; i <= A.length(); ++i) {
        if (tolower(A[i]) == tolower(B[b_index]))
            b_index++;
        if (b_index == B.length())
            return true;
    }
    return false;
}

std::vector<std::shared_ptr<Password>> Data::find(const std::string& t_name) {
    std::vector<std::shared_ptr<Password>> ret_vec{};
    for(auto & pass : this->passwords){
        if (substr(pass->getName(), t_name)){
            ret_vec.emplace_back(pass);
        }
    }
    return ret_vec;
}

std::vector<std::shared_ptr<Password>> Data::getData() {
    return this->passwords;
}

void Data::setValid(bool t_valid) {
    this->valid = t_valid;
}

bool Data::getValid() {
    return this->valid;
}
