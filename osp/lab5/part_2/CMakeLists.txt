cmake_minimum_required(VERSION 3.15)
project(part_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(part_2 main.c threads.c threads.h)