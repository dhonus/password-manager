//
// Created by daniel on 4/23/22.
//

#include <iostream>
#include "PasswordGenerator.h"
#include "random"

std::string PasswordGenerator::generate() {
    // initiate std::mt19937 with random num
    std::random_device rd;
    rng_type rng(rd());;

    // resulting pass vec
    std::string result;
    result.reserve(length);

    std::uniform_int_distribution<int> typeDist(0, choices.size() - 1);

    std::vector<char> lower {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    std::vector<char> syms { '+', '-', '*', '/', '!', '@', '$', '%', '^', ',' , '.', '=', '(',')','[',']'};
    std::vector<char> nums {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    char adding;
    while(result.size() < length)
    {
        switch (this->choices[typeDist(rng)])
        {
            case 'n':
                adding = nums[std::uniform_int_distribution<char>(0,nums.size() - 1)(rng)];
                break;
            case 'l':
                adding = lower[std::uniform_int_distribution<char>(0,lower.size() - 1)(rng)];
                break;
            case 'u':
                adding = toupper(lower[std::uniform_int_distribution<char>(0,lower.size() - 1)(rng)]);
                break;
            case 's':
                adding = syms[std::uniform_int_distribution<char>(0,syms.size() - 1)(rng)];
                break;
        }
        result += adding;
    }
    return result;
}

PasswordGenerator::PasswordGenerator(size_t t_len, bool t_lower, bool t_upper, bool t_syms) {
    if (t_len < 4)
        this->length = 4;
    else this->length = t_len;

    if (t_lower)
        this->choices.push_back('l');
    if(t_upper)
        this->choices.push_back('u');
    if(t_syms)
        this->choices.push_back('s');
}
