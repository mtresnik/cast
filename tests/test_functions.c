#include <stdio.h>
#include <malloc.h>
#include "test_functions.h"
#include "../include/cast/functions.h"
#include "../include/cast/util.h"


int testGetKeys() {
    char *testName = "testGetKeys";
    printf("---------------%s--------------\n", testName);
    struct StringArray entries = getFunctionEntryKeys();
    printStringArray(entries);
    return 0;
}


int testFunctions() {
    testGetKeys();
    return 0;
}