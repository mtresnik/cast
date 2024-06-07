

#include <stdio.h>
#include "test_util.h"
#include "../include/cast/util.h"

int testUtilAppendChar() {
    char *testName = "testUtilAppendChar";
    printf("---------------%s--------------\n", testName);
    char* testString = "abc";
    char* appended = appendChar(testString, '1');
    printf("appended: %s\n", appended);
    return 0;
}

int testUtilRemoveChar1() {
    char *testName = "testUtilRemoveChar1";
    printf("---------------%s--------------\n", testName);
    char *testString = "abc";
    char *removed = removeChar(testString, 'b');
    printf("removed: %s\n", removed);
    return 0;
}

int testUtilRemoveChar2() {
    char *testName = "testUtilRemoveChar2";
    printf("---------------%s--------------\n", testName);
    char *testString = "acbc ccc c c c  cccc";
    char *removed = removeChar(testString, 'c');
    printf("removed: %s\n", removed);
    return 0;
}


int testUtilRemoveChar3() {
    char *testName = "testUtilRemoveChar3";
    printf("---------------%s--------------\n", testName);
    char *testString = "acbc ccc c c c  cccc";
    char *removed = removeChar(testString, 'd');
    printf("removed: %s\n", removed);
    return 0;
}

int testUtilRemainingStrings1() {
    char *testName = "testUtilRemainingStrings1";
    printf("---------------%s--------------\n", testName);
    char* base = "abacd";
    char* replace = "ba";
    struct StringArray remaining = findRemainingStrings(base, replace);
    printStringArray(remaining);
    return 0;
}

int testUtilRemainingStrings2() {
    char *testName = "testUtilRemainingStrings2";
    printf("---------------%s--------------\n", testName);
    char* base = "abJcd";
    char* replace = "ab";
    struct StringArray remaining = findRemainingStrings(base, replace);
    printStringArray(remaining);
    return 0;
}

int testUtilRemainingStrings3() {
    char *testName = "testUtilRemainingStrings3";
    printf("---------------%s--------------\n", testName);
    char* base = "abacd";
    char* replace = "cd";
    struct StringArray remaining = findRemainingStrings(base, replace);
    printStringArray(remaining);
    return 0;
}

int testUtil() {
    testUtilAppendChar();
    testUtilRemoveChar1();
    testUtilRemoveChar2();
    testUtilRemoveChar3();
    testUtilRemainingStrings1();
    testUtilRemainingStrings2();
    testUtilRemainingStrings3();
    return 0;
}