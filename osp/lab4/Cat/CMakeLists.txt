cmake_minimum_required(VERSION 3.10)
project(Cat C)

set(CMAKE_C_STANDARD 11)

add_executable(Cat main.c cat.h cat.c)