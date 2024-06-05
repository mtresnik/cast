

#ifndef CAST_FUNCTIONS_H
#define CAST_FUNCTIONS_H
#define MAX_SIZE_FUNCTION_MAP (100)

#include "operation.h"

struct FunctionEntry {
    char* key;
    struct Operation (*value) (struct OperationArray operations);
};

struct StringArray getFunctionEntryKeys();

int getIndexOfFunction(char* key);

int containsKeyOfFunction(char* key);

struct Operation (*getFunctionEntryValue(char* key)) (struct OperationArray operations);

#endif //CAST_FUNCTIONS_H
