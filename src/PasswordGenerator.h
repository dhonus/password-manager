//
// Created by daniel on 4/23/22.
//

#ifndef CPP_PROJECT_PASSWORDGENERATOR_H
#define CPP_PROJECT_PASSWORDGENERATOR_H


#include <string>
#include <random>

class PasswordGenerator {
private:
    size_t length;
    typedef std::mt19937 rng_type;
    std::vector<int> choices = {'n'};
public:
    PasswordGenerator(size_t t_len, bool t_lower, bool t_upper, bool t_syms);
    std::string generate();
};

#endif //CPP_PROJECT_PASSWORDGENERATOR_H
