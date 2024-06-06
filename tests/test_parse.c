

#include <stdio.h>
#include <malloc.h>
#include "test_parse.h"
#include "../include/cast/parser.h"

void testParseNumbers() {
    char *testName = "testParseNumbers";
    printf("---------------%s--------------\n", testName);
    char* toParse = "20.0 + 50.0";
    struct Operation parsed = parse(toParse);
    printOperation(parsed);
    free(parsed.values);
}

void testParseVariables() {
    char *testName = "testParseVariables";
    printf("---------------%s--------------\n", testName);
    char* toParse = "abc + e + df";
    struct Operation parsed = parse(toParse);
    printOperation(parsed);
    free(parsed.values);
}

void testParseMultiplication() {
    char *testName = "testParseMultiplication";
    printf("---------------%s--------------\n", testName);
    char* toParse = "2x + 5";
    struct Operation parsed = parse(toParse);
    printOperation(parsed);
    free(parsed.values);
}

void testParseParentheses() {
    char *testName = "testParseParentheses";
    printf("---------------%s--------------\n", testName);
    char* toParse = "(5.0) + 6";
    struct Operation parsed = parse(toParse);
    printOperation(parsed);
    free(parsed.values);
}

void testParseParentheses2() {
    char *testName = "testParseParentheses2";
    printf("---------------%s--------------\n", testName);
    char* toParse = "(5.0) + (6)";
    struct Operation parsed = parse(toParse);
    printOperation(parsed);
    free(parsed.values);
}

int testParse() {
    testParseNumbers();
    testParseVariables();
    testParseMultiplication();
    testParseParentheses();
    testParseParentheses2();
    return 0;
}