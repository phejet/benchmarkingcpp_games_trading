cmake_minimum_required(VERSION 2.8.11)

PROJECT(fizzbuzz)

if (UNIX)
    add_definitions("-std=c++11")
    add_definitions("-Wall")
    add_definitions("-Werror")
	add_definitions("-fno-omit-frame-pointer")
	add_definitions("-g")
	add_definitions("-O3")
endif()

add_subdirectory(benchmark)
add_subdirectory(fizzbuzz_server)
add_subdirectory(fizzbuzz_benchmark)
