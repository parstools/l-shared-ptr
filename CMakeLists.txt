cmake_minimum_required(VERSION 3.20)
project(l_shared_ptr)

set(CMAKE_CXX_STANDARD 14)

add_executable(l_shared_ptr main.cpp LShared.cpp tree.cpp tree.h weak.cpp weak.h)
