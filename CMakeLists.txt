cmake_minimum_required(VERSION 3.23)
project(password_manager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "-fexceptions -lsodium -lcryptopp -fsanitize=address -g")

add_compile_options(-Wall -Wextra -pedantic)

add_executable(password_manager src/main.cpp src/Password.cpp src/Password.h src/Data.cpp src/Data.h
                src/pugixml/pugiconfig.hpp src/pugixml/pugixml.cpp src/pugixml/pugixml.hpp src/User.cpp src/User.h src/PasswordGenerator.cpp src/PasswordGenerator.h src/Vault.cpp src/Vault.h)
