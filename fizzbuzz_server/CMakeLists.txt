cmake_minimum_required(VERSION 2.8)

set(MODULE_NAME fizzbuzz_server)
project(${MODULE_NAME})

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(FILES 
    application.h
    application.cc
    fizzbuzz_server_main.cc
    network.h
    network.cc
    timing.h
    timing.cc)

add_executable(${MODULE_NAME} ${FILES})