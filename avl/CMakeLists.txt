cmake_minimum_required(VERSION 3.5)

project(avl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(avl main.cpp)

install(TARGETS avl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
