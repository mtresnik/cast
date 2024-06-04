cmake_minimum_required(VERSION 3.28)
project(cast C)

set(CMAKE_C_STANDARD 23)

add_executable(cast main.c
        include/cast/operation.h
        src/operation.c
        tests/test_bulk.h
        tests/test_bulk.c
        include/cast/token.h
        src/token.c
)
