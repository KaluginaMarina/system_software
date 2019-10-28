cmake_minimum_required(VERSION 3.10)
project(part_1_server C)

set(CMAKE_C_STANDARD 99)

add_executable(part_1_server main.c server.c server.h)