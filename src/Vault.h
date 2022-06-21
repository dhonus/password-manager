//
// Created by daniel on 27.5.22.
//

#ifndef CPP_PROJECT_VAULT_H
#define CPP_PROJECT_VAULT_H
#include <cstdio>
#include <sodium.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <optional>
#include "Data.h"
#include <string>
#include <sstream>
#include <iostream>
#include "pugixml.hpp"
#include <termios.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>

class Vault {
private:
    std::string vault_password;
    std::string filename;
    std::optional<std::stringstream> decrypt(std::string t_source_file);
public:
    Vault();
    std::shared_ptr<Data> assembleFileFromData(std::string t_path);
    int save(std::shared_ptr<Data> t_data);
    bool file_exists(const std::string &file);
    std::string getPath();
    void setPath(const std::string &t_path) ;
};

#endif //CPP_PROJECT_VAULT_H
