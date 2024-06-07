#include <stdio.h>
#include <malloc.h>
#include "../include/cast/variables.h"
#include "../include/cast/util.h"

struct VariableEntry* variableEntries[MAX_SIZE_VARIABLE_MAP];
int initializedVariableMap = false;
int initVariableMap();

struct StringArray getVariableEntryKeys() {
    if (initializedVariableMap == false) {
        initVariableMap();
    }
    int count = 0;
    for (int i = 0; i < MAX_SIZE_VARIABLE_MAP; ++i) {
        if(variableEntries[i] != NULL) {
            count++;
        }
    }
    int newSize = count * sizeof(char*);
    char** currArray = malloc(newSize); // assume size
    count = 0;
    for (int i = 0; i < MAX_SIZE_VARIABLE_MAP; ++i) {
        if(variableEntries[i] != NULL) {
            currArray[count] = cloneString(variableEntries[i]->key);
            count++;
        }
    }
    struct StringArray retArray;
    retArray.numStrings = count;
    retArray.strings = currArray;
    return retArray;
}

int getIndexOfVariable(char* key) {
    return safeMod(getHash(key), MAX_SIZE_VARIABLE_MAP);
}

int containsKeyOfVariable(char* key) {
    if (initializedVariableMap == false) {
        initVariableMap();
    }
    int index = getIndexOfVariable(key);
    return variableEntries[index] != NULL;
}

int putVariableEntry(struct VariableEntry entry){
    int index = getIndexOfVariable(entry.key);
    variableEntries[index] = malloc(sizeof(struct VariableEntry));
    *variableEntries[index] = entry;
    return 0;
}

struct VariableEntry newVariableEntry(char* name){
    struct VariableEntry entry;
    entry.key = cloneString(name);
    entry.variable = Variable(name);
    putVariableEntry(entry);
    return entry;
}

int initVariableMap() {
    putVariableEntry(newVariableEntry("e"));
    putVariableEntry(newVariableEntry("i"));
    return 0;
}