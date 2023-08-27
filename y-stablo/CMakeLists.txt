cmake_minimum_required(VERSION 3.5)

project(y-stablo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(y-stablo main.cpp x_stablo.hpp avl.hpp)

install(TARGETS y-stablo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
