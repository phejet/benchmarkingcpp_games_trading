cmake_minimum_required(VERSION 2.8)

set(MODULE_NAME server)
project(${MODULE_NAME})

add_executable(${MODULE_NAME} server.cc)