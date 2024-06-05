#include <stdio.h>
#include "test_variables.h"
#include "../include/cast/util.h"
#include "../include/cast/variables.h"

int testVariableMap() {
    char *testName = "testVariableMap";
    printf("---------------%s--------------\n", testName);
    struct StringArray entries = getVariableEntryKeys();
    printStringArray(entries);
    return 0;
}

int testVariables() {
    testVariableMap();
    return 0;
}