cmake_minimum_required(VERSION 3.16)
project(help118 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(help118
        waveform.c
        io.c
        main.c)
