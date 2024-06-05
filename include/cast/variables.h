

#ifndef CAST_VARIABLES_H
#define CAST_VARIABLES_H
#define MAX_SIZE_VARIABLE_MAP (100)

#include "operation.h"

struct VariableEntry {
    char* key;
    struct Operation variable;
};

struct StringArray getVariableEntryKeys();

int getIndexOfVariable(char* key);

int containsKeyOfVariable(char* key);


#endif //CAST_VARIABLES_H
