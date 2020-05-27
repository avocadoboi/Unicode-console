cmake_minimum_required(VERSION 3.17)
project(UnicodeConsole)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    UnicodeConsole
    Console.hpp
    Console.cpp
    example.cpp
)
