//
// Created by daniel on 4/21/22.
//

#include <iostream>
#include "memory"
#include "Data.h"
#include "Password.h"
#include "PasswordGenerator.h"
#include "User.h"
#include <algorithm>
#include <vector>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\x1B[34m"
#define BOLD "\x1B[1m"

User::User(std::shared_ptr<Vault> t_vault) {
    this->vault = t_vault;
    this->needed = true;
}

bool User::getNeeded() const {
    return this->needed;
}

void User::setNeeded(const bool t_needed){
    this->needed = t_needed;
}

int User::run(std::shared_ptr<Data> data) {
    short command;
    std::string temp{};
    while (this->getNeeded()){
        std::cout << BLUE <<
                  "- - - - - - - - - - - - - - - - -\n" << RESET <<
                  BOLD << "[ 0 ] " << RESET << "list all passwords\n" <<
                  BOLD << "[ 1 ] " << RESET << "search - finds entries by name\n" <<
                  BOLD << "[ 2 ] " << RESET << "add new entry\n" <<
                  BOLD << "[ 3 ] " << RESET << "remove entry\n" <<
                  BOLD << "[ 4 ] " << RESET << "save\n"  <<
                  BOLD << "[ 5 ] " << RESET << "save and exit - quits program saving changes\n" <<
                  BOLD << "[ 6 ] " << RESET << "exit - quits program without changes\n" << BLUE
                  "- - - - - - - - - - - - - - - - -" << RESET << std::endl;
        std::cin >> command;
        if (command < 0 || command > 9) continue;
        switch (command) {
            case 0: {
                std::cout << "Found:\n";
                auto pass = data->getData();
                if (!pass.empty())
                    for (auto & password : pass){
                        password->prettyPrint();
                        std::cout << "\n";
                    }
                else std::cout << YELLOW << "No entries found.\n" << RESET << std::flush;
                break;
            }
            case 1: {
                std::cout << "Search for: " << std::flush;
                std::cin >> temp;
                std::cout << "Found: ";
                auto pass = data->find(temp);
                if (!pass.empty())
                    for (auto & password : pass){
                        password->prettyPrint();
                        std::cout << "\n";
                    }
                else std::cout << YELLOW << "No entries found for " << temp;
                std::cout << "\n" << RESET << std::flush;
                break;
            }
            case 2:{
                std::cout << "Name: " << std::flush;
                std::cin >> temp;
                std::cout << "\nUsername: " << std::flush;
                std::string username;
                std::cin >> username;
                std::cout << "\nGenerate password? [Y,n]: " << std::flush;
                std::string passQuestion;
                std::cin >> passQuestion;
                std::vector<std::string> no {"n", "N","No","NO","nO","no"};
                std::string pass{};

                if (std::find(no.begin(), no.end(), passQuestion) != no.end()){
                    std::cout << "\nEnter new password: " << std::flush;
                    std::cin >> pass;
                } else{
                    bool upper, lower, syms;
                    std::string choiceTemp{};

                    std::cout << "\nUppercase? [Y,n]" << std::flush;
                    std::cin >> choiceTemp;
                    if (std::find(no.begin(), no.end(), choiceTemp) != no.end()){
                        upper = false;
                    } else upper = true;

                    std::cout << "Lowercase? [Y,n]" << std::flush;
                    std::cin >> choiceTemp;
                    if (std::find(no.begin(), no.end(), choiceTemp) != no.end()){
                        lower = false;
                    } else lower = true;

                    std::cout << "Special symbols? [Y,n]" << std::flush;
                    std::cin >> choiceTemp;
                    if (std::find(no.begin(), no.end(), choiceTemp) != no.end()){
                        syms = false;
                    } else syms = true;
                    int length;
                    std::cout << "Length of new password? n >= 4 " << std::flush;
                    std::cin >> length;
                    std::cout << length;

                    PasswordGenerator pg(length, upper, lower, syms);
                    pass = pg.generate();
                }

                std::shared_ptr<Password> newPass = std::make_shared<Password>(temp, username, pass);
                data->insert(newPass);
                break;
            }
            case 3:
            {
                auto all = data->find(temp);
                if (all.empty()){
                    std::cout << YELLOW << "No entries found for " << temp;
                    std::cout << "\n" << RESET << std::flush;
                    break;
                }
                for (size_t i = 0; i < all.size(); ++i){
                    std::cout << i << ": ";
                    all[i]->prettyPrint();
                    std::cout << "\n";
                }
                std::cout << "-1: Cancel removal\n";

                int to_remove_index = -1;
                std::cin >> to_remove_index;
                if(to_remove_index == -1){
                    break;
                }
                data->remove(all[to_remove_index]);

               break;
            }
            case 4:
            {
                this->vault->save(data);
                std::cout << GREEN << "Saved succesfully!\n" << RESET;
                break;
            }
            case 5:
                this->vault->save(data);
                std::cout << GREEN << "Saved succesfully!\n" << RESET;
                this->setNeeded(false);
                break;
            case 6:
                this->setNeeded(false);
                break;
            default:
                break;
        }
    }
    return EXIT_SUCCESS;
}
