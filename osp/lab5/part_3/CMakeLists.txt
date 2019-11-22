cmake_minimum_required(VERSION 3.15)
project(part_3 C)

set(CMAKE_C_STANDARD 99)

add_executable(part_3 main.c server.c server.h client.c)
add_executable(part_3 server.h client.c)