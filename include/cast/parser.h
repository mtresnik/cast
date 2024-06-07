#ifndef CAST_PARSER_H
#define CAST_PARSER_H
#include "operation.h"
#include "token.h"

struct Intermediate {
    int startIndex;
    int endIndex;
    int type;
    struct TokenArray tokens;
    char* name;
    int numInner;
    struct Intermediate* inner;
};

struct IntermediateArray {
    int numInner;
    int arraySize;
    struct Intermediate* inner;
};

struct Intermediate nullIntermediate();

struct TokenSet {
    int startIndex;
    int endIndex;
    int type;
    struct TokenArray tokens;
    char* representation;
};

struct TokenSetArray {
    int numValues;
    int arraySize;
    struct TokenSet* values;
};

int printTokenSet(struct TokenSet tokenSet);

int printTokenSets(struct TokenSetArray tokenSets);

struct TokenSetArray appendTokenSetArray(struct TokenSetArray base, struct TokenSet tokenSet);

struct Operation parseOperation(char* inputString);

#endif //CAST_PARSER_H
