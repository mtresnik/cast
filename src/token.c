#define NULL ((void *)0)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "../include/cast/token.h"
#include "../include/cast/util.h"

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
    retArray.values = malloc(other.numValues * sizeof(struct Token));
    for (int i = 0; i < other.numValues; ++i) {
        retArray.values[i] = cloneToken(other.values[i]);
    }
    return retArray;
}

struct TokenArray appendTokenArray(struct TokenArray base, struct Token token){
    if (base.values == NULL || base.numValues == 0) {
        struct TokenArray retArray;
        retArray.numValues = 1;
        retArray.values = malloc(retArray.numValues * sizeof(struct Token));
        retArray.values[0] = token;
        return retArray;
    }
    struct TokenArray retArray;
    retArray.numValues = base.numValues + 1;
    retArray.values = realloc(base.values, retArray.numValues * sizeof(struct Token));
    retArray.values[base.numValues] = token;
    return retArray;
}

struct TokenArray appendTokenArrayAndFree(struct TokenArray base, struct Token token) {
    struct TokenArray ret = appendTokenArray(base, token);
    if (base.values != NULL) {
        free(base.values);
    }
    return ret;
}


struct TokenArray concatTokenArrays(struct TokenArray base, struct TokenArray other) {
    if (base.numValues == 0 || base.values == NULL) {
        return cloneTokenArray(other);
    }
    if (other.numValues == 0 || other.values == NULL) {
        return cloneTokenArray(base);
    }
    struct TokenArray retArray;
    retArray.numValues = base.numValues + other.numValues;
    retArray.values = malloc(retArray.numValues * sizeof(struct Token));
    memcpy(retArray.values, base.values, base.numValues * sizeof(struct Token));
    memcpy(retArray.values + base.numValues, other.values, other.numValues * sizeof(struct Token));
    return retArray;
}

struct TokenArray concatTokenArraysAndFree(struct TokenArray base, struct TokenArray other) {
    struct TokenArray ret = concatTokenArrays(base, other);
    if (base.values != NULL) {
        free(base.values);
    }
    if (other.values != NULL) {
        free(other.values);
    }
    return ret;
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
    retToken.representation = representation;
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
    // free(tokenString);
    return 0;
}

int printTokens(struct TokenArray t) {
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