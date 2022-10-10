cmake_minimum_required(VERSION 3.17)
project(UnicodeConsole)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	add_compile_options(/utf-8)
endif ()

add_library(unicode_console 
    include/unicode_console.hpp
    source/unicode_console.cpp)

target_include_directories(unicode_console PUBLIC include)

target_compile_features(unicode_console PRIVATE cxx_std_17)

add_subdirectory(examples)
