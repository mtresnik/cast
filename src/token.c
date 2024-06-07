#define NULL ((void *)0)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "../include/cast/token.h"
#include "../include/cast/util.h"

size_t sizeOfToken(struct Token token) {
    return (token.endIndex - token.startIndex + 1) * sizeof(char) + sizeof(struct Token);
}

size_t sizeOfTokenArray(struct TokenArray array) {
    size_t ret = 0;
    for (int i = 0; i < array.numValues; ++i) {
        ret += sizeOfToken(array.values[i]);
    }
    return ret;
}


struct TokenArray nullTokenArray() {
    struct TokenArray retArray = {
            0,
            0,
            NULL
    };
    return retArray;
}

struct TokenArray singleTokenArray(struct Token token) {
    struct TokenArray retArray;
    retArray.numValues = 1;
    retArray.arraySize = 1;
    retArray.values = malloc(sizeOfToken(token));
    if (retArray.values == NULL) {
        fprintf(stderr, "singleTokenArray Memory allocation failed!\n");
        exit(1);
    }
    retArray.values[0] = token;
    return retArray;
}

struct TokenArray doubleTokenArray(struct Token left, struct Token right) {
    struct TokenArray retArray;
    retArray.numValues = 2;
    retArray.arraySize = 2;
    retArray.values = malloc(2 * sizeof(struct Token));
    retArray.values[0] = left;
    retArray.values[1] = right;
    return retArray;
}

struct Token cloneToken(struct Token other) {
    struct Token retToken;
    retToken.startIndex = other.startIndex;
    retToken.endIndex = other.endIndex;
    retToken.type = other.type;
    retToken.representation = cloneString(other.representation);
    return retToken;
}

struct TokenArray cloneTokenArray(struct TokenArray other){
    struct TokenArray retArray;
    retArray.numValues = other.numValues;
    retArray.arraySize = other.arraySize;
    retArray.values = malloc(other.arraySize * sizeof(struct Token));
    for (int i = 0; i < other.numValues; ++i) {
        retArray.values[i] = cloneToken(other.values[i]);
    }
    return retArray;
}

struct TokenArray appendTokenArray(struct TokenArray base, struct Token token){
    if (base.values == NULL || base.numValues == 0) {
        struct TokenArray retArray;
        retArray.numValues = 1;
        retArray.arraySize = 1;
        retArray.values = malloc(retArray.numValues * sizeof(struct Token));
        retArray.values[0] = token;
        return retArray;
    }
    int expectedIndex = base.numValues;
    if (expectedIndex < base.arraySize) {
        base.values[expectedIndex] = token;
        base.numValues++;
        return base;
    }

    struct TokenArray retArray;
    retArray.numValues = base.numValues + 1;
    retArray.arraySize = maxInt(retArray.numValues, ceilf(1.62 * retArray.numValues));
    size_t newSize = retArray.arraySize * sizeof(struct Token);
    retArray.values = realloc(base.values, newSize);
    retArray.values[base.numValues] = token;
    return retArray;
}

struct TokenArray concatTokenArrays(struct TokenArray base, struct TokenArray other) {
    if (base.numValues == 0 || base.values == NULL) {
        return other;
    }
    if (other.numValues == 0 || other.values == NULL) {
        return base;
    }
    struct TokenArray retArray;
    retArray.numValues = base.numValues + other.numValues;
    retArray.arraySize = retArray.numValues;
    size_t newSize = 0;
    for (int i = 0; i < base.numValues; ++i) {
        newSize += sizeOfToken(base.values[i]);
    }
    for (int i = 0; i < other.numValues; i++) {
        newSize += sizeOfToken(other.values[i]);
    }
    retArray.values = malloc(newSize);
    if (retArray.values == NULL) {
        fprintf(stderr, "concatTokenArrays Memory allocation failed!\n");
        exit(1);
    }
    int outerIndex = 0;
    for (int i = 0; i < base.numValues; i++) {
        retArray.values[outerIndex] = base.values[i];
        outerIndex++;
    }
    for (int i = 0; i < other.numValues; i++) {
        retArray.values[outerIndex] = other.values[i];
        outerIndex++;
    }
    return retArray;
}

struct Token singleIndex(int index, int type) {
    struct Token retToken;
    retToken.startIndex = index;
    retToken.endIndex = index;
    retToken.type = type;
    retToken.representation = NULL;
    return retToken;
}

struct Token nullIndex(int type, char* representation) {
    struct Token retToken;
    retToken.startIndex = -1;
    retToken.endIndex = -1;
    retToken.type = type;
    retToken.representation = cloneString(representation);
    return retToken;
}

struct Token convert(struct Token other, int type){
    struct Token retToken;
    retToken.startIndex = other.startIndex;
    retToken.endIndex = other.endIndex;
    retToken.type = type;
    retToken.representation = other.representation;
    return retToken;
}

char* toStringToken(struct Token t) {
    char* retString = malloc(32 * sizeof(char));
    char* representation = "NULL";
    if (t.representation != NULL) {
        representation = t.representation;
    }
    sprintf(retString, "[[%d,%d] : %d : %s]", t.startIndex, t.endIndex, t.type, representation);
    return retString;
}

int printToken(struct Token t) {
    char* tokenString = toStringToken(t);
    printf("%s\n", tokenString);
    free(tokenString);
    return 0;
}

int printTokens(struct TokenArray t) {
    printf("TokenArray Num Values:%d\n", t.numValues);
    for (int i = 0; i < t.numValues; ++i) {
        printToken(t.values[i]);
    }
    return 0;
}

int equalsToken(struct Token one, struct Token other) {
    if (one.startIndex != other.startIndex) {
        return false;
    }
    if (one.endIndex != other.endIndex) {
        return false;
    }
    if (one.representation == NULL && other.representation != NULL) {
        return false;
    }
    if (one.representation != NULL && other.representation == NULL) {
        return false;
    }
    if (one.representation == NULL && other.representation == NULL) {
        return true;
    }
    char *str1 = one.representation;
    char *str2 = other.representation;
    int retValue = strcmp(str1, str2);
    return retValue == 0;
}

int containsToken(struct TokenArray oneArray, struct Token other){
    for (int i = 0; i < oneArray.numValues; ++i) {
        struct Token token = oneArray.values[i];
        if (equalsToken(token, other)) {
            return true;
        }
    }
    return false;
}

int containsAllTokens(struct TokenArray oneArray, struct TokenArray otherArray) {
    if (oneArray.numValues != otherArray.numValues) {
        return false;
    }
    for (int i = 0; i < oneArray.numValues; ++i) {
        struct Token token = oneArray.values[i];
        if (containsToken(otherArray, token) == false) {
            return false;
        }
    }
    return true;
}