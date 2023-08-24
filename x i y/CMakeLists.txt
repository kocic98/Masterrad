cmake_minimum_required(VERSION 3.5)

project(untitled1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(untitled1 main.cpp AVL.h xfast.h)

install(TARGETS untitled1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
