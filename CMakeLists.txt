cmake_minimum_required(VERSION 2.8.11)

PROJECT(fizzbuzz)

if (UNIX)
    add_definitions("-std=c++11")
    add_definitions("-Wall")
    add_definitions("-Werror")
endif()

add_subdirectory(fizzbuzz_server)
