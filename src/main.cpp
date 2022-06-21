#include <iostream>
#include <filesystem>
#include "Data.h"
#include "Password.h"
#include "User.h"
#include "Vault.h"

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"

bool file_exists(const std::string &file)
{
    std::ifstream infile(file);
    return infile.good();
}

int main(int argc, char *argv[]) {

    std::string path{};
    if (argc == 1){
        std::cout << "Please enter the absolute path to your password vault: ";
        getline(std::cin, path);
    } else {
        path = argv[1];
    }

    if (!file_exists(path)){
        std::cout << YELLOW << "No such file or directory. A new file will be created.\n" << RESET;
    }

    std::cout << " .--.         opening: " << path << "\n" <<
                 "/.-. '----------.\n"
                 "\\'-' .--\"--\"\"-\"-'\n"
                 " '--' \n";

    std::shared_ptr<Data> data = std::make_shared<Data>();
    std::shared_ptr<Vault> vault = std::make_shared<Vault>();
    std::unique_ptr<User> user = std::make_unique<User>(vault);

    data = std::move(vault->assembleFileFromData(path));
    if (!data->getData().empty()){
        std::cout << GREEN << "Successfully unlocked the vault.\n" << RESET;
    } else {
        std::cout << YELLOW << "Created a new vault.\n" << RESET;
    }

    if(!user->run(data)){
        std::cout << "Goodbye...\n";
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
