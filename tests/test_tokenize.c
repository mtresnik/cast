#include <stdio.h>
#include <malloc.h>
#include "test_tokenize.h"
#include "../include/cast/tokenizer.h"

int testTokenizeAll() {
    char *testName = "testTokenizeAll";
    printf("---------------%s--------------\n", testName);
    char* inputString = "sin(123.0) + abc * e / 2 ^ x";
    struct TokenArray tokens = tokenize(inputString);
    printTokens(tokens);
    free(tokens.values);
    return 0;
}

int testTokenize() {
    testTokenizeAll();
    return 0;
}

