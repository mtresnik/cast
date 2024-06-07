#include <stdio.h>
#include <malloc.h>
#include "test_tokens.h"
#include "../include/cast/token.h"

int testTokensAppend() {
    char *testName = "testTokensAppend";
    printf("---------------%s--------------\n", testName);
    struct Token token;
    token = singleIndex(2, NUMBER_TOKEN);
    struct TokenArray tokenArray = nullTokenArray();
    tokenArray = appendTokenArray(tokenArray, token);
    printTokens(tokenArray);
    struct Token token2;
    token2 = singleIndex(3, VARIABLE_TOKEN);
    tokenArray = appendTokenArray(tokenArray, token2);
    printTokens(tokenArray);
    struct Token token3;
    token3 = singleIndex(3, VARIABLE_TOKEN);
    tokenArray = appendTokenArray(tokenArray, token3);
    printTokens(tokenArray);
    struct Token token4;
    token4 = singleIndex(4, VARIABLE_TOKEN);
    tokenArray = appendTokenArray(tokenArray, token4);
    printTokens(tokenArray);
    struct Token token5;
    token5 = singleIndex(5, VARIABLE_TOKEN);
    tokenArray = appendTokenArray(tokenArray, token5);
    printTokens(tokenArray);
    free(tokenArray.values);
    return 0;
}

int testTokens() {
    testTokensAppend();
    return 0;
}