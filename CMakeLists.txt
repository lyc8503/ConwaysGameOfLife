cmake_minimum_required(VERSION 3.20)
project(ConwaysGameOfLife C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ConwaysGameOfLife
        main.c round.c chunk.c board.c)
