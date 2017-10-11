cmake_minimum_required(VERSION 2.8.11)

set(MODULE_NAME fizzbuzz_benchmark)
project(${MODULE_NAME})

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(FILES 
    main.cc
)

add_executable(${MODULE_NAME} ${FILES})
target_link_libraries(${MODULE_NAME} benchmark)
