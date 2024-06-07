#include <stdio.h>
#include "test_variables.h"
#include "../include/cast/util.h"
#include "../include/cast/variables.h"

int testVariablesMap() {
    char *testName = "testVariablesMap";
    printf("---------------%s--------------\n", testName);
    struct StringArray entries = getVariableEntryKeys();
    printStringArray(entries);
    return 0;
}

int testVariables() {
    testVariablesMap();
    return 0;
}