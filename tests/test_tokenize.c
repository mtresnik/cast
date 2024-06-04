#include <stdio.h>
#include "test_tokenize.h"
#include "../include/cast/tokenizer.h"

int testNumbers() {
    char *testName = "testNumbers";
    printf("---------------%s--------------\n", testName);
    struct TokenArray tokens = tokenize("123.5 + 1 + abc + 2");
    printf("tokens Size: %d\n", tokens.numValues);
    printTokens(tokens);
    return 0;
}

int testTokenize() {
    testNumbers();
    return 0;
}

