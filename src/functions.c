#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "../include/cast/functions.h"
#include "../include/cast/util.h"


struct FunctionEntry* functionEntries[MAX_SIZE_FUNCTION_MAP];
int initializedFunctionMap = 0;
int initFunctionMap();

struct StringArray getFunctionEntryKeys() {
    if (initializedFunctionMap == 0) {
        initFunctionMap();
    }
    int count = 0;
    for (int i = 0; i < MAX_SIZE_FUNCTION_MAP; ++i) {
        if(functionEntries[i] != NULL) {
            count++;
        }
    }
    size_t newSize = count * sizeof(char*);
    char** currArray = malloc(newSize); // assume size
    count = 0;
    for (int i = 0; i < MAX_SIZE_FUNCTION_MAP; ++i) {
        if(functionEntries[i] != NULL) {
            currArray[count] = cloneString(functionEntries[i]->key);
            count++;
        }
    }
    struct StringArray retArray;
    retArray.numStrings = count;
    retArray.strings = currArray;
    return retArray;
}

int getIndexOfFunction(char* key) {
    return safeMod(getHash(key), MAX_SIZE_FUNCTION_MAP);
}

int containsKeyOfFunction(char* key) {
    if (initializedFunctionMap == 0) {
        initFunctionMap();
    }
    int index = getIndexOfFunction(key);
    return functionEntries[index] != NULL;
}

int putFunctionEntry(struct FunctionEntry entry) {
    int index = getIndexOfFunction(entry.key);
    functionEntries[index] = malloc(sizeof(struct FunctionEntry));
    *functionEntries[index] = entry;
    return 0;
}

struct Operation (*getFunctionEntryValue(char* key)) (struct OperationArray operations)  {
    int index = getIndexOfFunction(key);
    if (functionEntries[index] != NULL) {
        return functionEntries[index]->value;
    } else {
        return NULL;
    }
}

struct FunctionEntry newFunctionEntry(char* key, struct Operation (*value) (struct OperationArray operations)) {
    struct FunctionEntry sinEntry;
    sinEntry.key = cloneString(key);
    sinEntry.value = value;
    return sinEntry;
}

struct Operation makeSin(struct OperationArray operations) {
    return Sin(operations.values[0]);
}

struct Operation makeCos(struct OperationArray operations) {
    return Cos(operations.values[0]);
}

struct Operation makeTan(struct OperationArray operations) {
    return Tan(operations.values[0]);
}

struct Operation makeArcSin(struct OperationArray operations) {
    return ArcSin(operations.values[0]);
}

struct Operation makeArcCos(struct OperationArray operations) {
    return ArcCos(operations.values[0]);
}

struct Operation makeArcTan(struct OperationArray operations) {
    return ArcTan(operations.values[0]);
}

struct Operation makeAbs(struct OperationArray operations) {
    return Abs(operations.values[0]);
}

struct Operation makeLog(struct OperationArray operations) {
    struct Operation* c = malloc(sizeof(struct Operation));
    *c = Constant(10.0 + 0*I);
    return Log(c, operations.values[0]);
}

struct Operation makeLn(struct OperationArray operations) {
    struct Operation* c = malloc(sizeof(struct Operation));
    *c = Constant(exp(1) + 0*I);
    return Log(c, operations.values[0]);
}

struct Operation makeLogBase(struct OperationArray operations) {
    return Log(operations.values[0], operations.values[1]);
}

int initFunctionMap(){
    putFunctionEntry(newFunctionEntry("sin", makeSin));
    putFunctionEntry(newFunctionEntry("cos", makeCos));
    putFunctionEntry(newFunctionEntry("tan", makeTan));
    putFunctionEntry(newFunctionEntry("arcsin", makeArcSin));
    putFunctionEntry(newFunctionEntry("arccos", makeArcCos));
    putFunctionEntry(newFunctionEntry("arctan", makeArcTan));
    putFunctionEntry(newFunctionEntry("abs", makeAbs));
    putFunctionEntry(newFunctionEntry("log", makeLog));
    putFunctionEntry(newFunctionEntry("ln", makeLn));
    putFunctionEntry(newFunctionEntry("log_", makeLogBase));
    initializedFunctionMap = 1;
    return 0;
}