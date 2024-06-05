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
        include/cast/tokenizer.h
        src/tokenizer.c
        include/cast/util.h
        src/util.c
        tests/test_tokenize.c
        tests/test_tokenize.h
        tests/test_util.c
        tests/test_util.h
        include/cast/functions.h
        src/functions.c
        tests/test_functions.c
        tests/test_functions.h
        include/cast/variables.h
        src/variables.c
        tests/test_variables.c
        tests/test_variables.h
)
