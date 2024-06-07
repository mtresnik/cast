#include <stdio.h>
#include <malloc.h>
#include "test_functions.h"
#include "../include/cast/functions.h"
#include "../include/cast/util.h"


int testFunctionsGetKeys() {
    char *testName = "testFunctionsGetKeys";
    printf("---------------%s--------------\n", testName);
    struct StringArray entries = getFunctionEntryKeys();
    printStringArray(entries);
    return 0;
}

int testFunctionsGenerate() {
    char *testName = "testFunctionsGenerate";
    printf("---------------%s--------------\n", testName);
    char* functionName = "sin";
    struct Operation (*value) (struct OperationArray operations) = getFunctionEntryValue(functionName);
    struct OperationArray operationArray;
    operationArray.numValues = 1;
    operationArray.values = malloc(operationArray.numValues * sizeof(struct Operation*));
    operationArray.values[0] = malloc(sizeof(struct Operation));
    *operationArray.values[0] = Variable("x");
    struct Operation result = value(operationArray);
    printOperation(result);
    free(operationArray.values);
}


int testFunctions() {
    testFunctionsGetKeys();
    testFunctionsGenerate();
    return 0;
}