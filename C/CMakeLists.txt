cmake_minimum_required(VERSION 3.20)
project(c_shared_ptr)

set(CMAKE_C_STANDARD 11)

add_executable(c_shared_ptr main.c CShared.c)
