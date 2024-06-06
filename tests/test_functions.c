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

int testGenerateFunction() {
    char *testName = "testGenerateFunction";
    printf("---------------%s--------------\n", testName);
    char* functionName = "sin";
    char* xString = "x";
    struct Operation (*value) (struct OperationArray operations) = getFunctionEntryValue(functionName);
    struct OperationArray operationArray;
    operationArray.numValues = 1;
    operationArray.values = malloc(operationArray.numValues * sizeof(struct Operation*));
    operationArray.values[0] = malloc(sizeof(struct Operation));
    *operationArray.values[0] = Variable(&xString);
    struct Operation result = value(operationArray);
    printOperation(result);
}


int testFunctions() {
    testGetKeys();
    testGenerateFunction();
    return 0;
}