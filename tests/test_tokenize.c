#include <stdio.h>
#include <malloc.h>
#include "test_tokenize.h"
#include "../include/cast/tokenizer.h"

int testAll() {
    char *testName = "testAll";
    printf("---------------%s--------------\n", testName);
    char* inputString = "sin(123.5 + iabei + - 123.5)";
    struct TokenArray tokens = tokenize(inputString);
    printTokens(tokens);
    free(tokens.values);
    return 0;
}

int testTokenize() {
    testAll();
    return 0;
}

