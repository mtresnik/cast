

#include <stdio.h>
#include "test_util.h"
#include "../include/cast/util.h"

int testAppendChar() {
    char *testName = "testAppendChar";
    printf("---------------%s--------------\n", testName);
    char* testString = "abc";
    char* appended = appendChar(testString, '1');
    printf("appended: %s\n", appended);
    return 0;
}

int testRemoveChar1() {
    char *testName = "testRemoveChar1";
    printf("---------------%s--------------\n", testName);
    char *testString = "abc";
    char *removed = removeChar(testString, 'b');
    printf("removed: %s\n", removed);
    return 0;
}

int testRemoveChar2() {
    char *testName = "testRemoveChar2";
    printf("---------------%s--------------\n", testName);
    char *testString = "acbc ccc c c c  cccc";
    char *removed = removeChar(testString, 'c');
    printf("removed: %s\n", removed);
    return 0;
}


int testRemoveChar3() {
    char *testName = "testRemoveChar3";
    printf("---------------%s--------------\n", testName);
    char *testString = "acbc ccc c c c  cccc";
    char *removed = removeChar(testString, 'd');
    printf("removed: %s\n", removed);
    return 0;
}

int testRemainingStrings1() {
    char *testName = "testRemainingStrings1";
    printf("---------------%s--------------\n", testName);
    char* base = "abacd";
    char* replace = "ba";
    struct StringArray remaining = findRemainingStrings(base, replace);
    printStringArray(remaining);
    return 0;
}

int testRemainingStrings2() {
    char *testName = "testRemainingStrings2";
    printf("---------------%s--------------\n", testName);
    char* base = "abJcd";
    char* replace = "ab";
    struct StringArray remaining = findRemainingStrings(base, replace);
    printStringArray(remaining);
    return 0;
}

int testRemainingStrings3() {
    char *testName = "testRemainingStrings3";
    printf("---------------%s--------------\n", testName);
    char* base = "abacd";
    char* replace = "cd";
    struct StringArray remaining = findRemainingStrings(base, replace);
    printStringArray(remaining);
    return 0;
}

int testUtil() {
    testAppendChar();
    testRemoveChar1();
    testRemoveChar2();
    testRemoveChar3();
    testRemainingStrings1();
    testRemainingStrings2();
    testRemainingStrings3();
    return 0;
}