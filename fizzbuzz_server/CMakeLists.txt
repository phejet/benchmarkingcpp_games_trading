cmake_minimum_required(VERSION 2.8.11)

set(MODULE_NAME fizzbuzz_server)
project(${MODULE_NAME})

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(FILES 
    main.cc
    timing.h
    timing.cc)

add_executable(${MODULE_NAME} ${FILES})
