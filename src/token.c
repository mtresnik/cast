#define NULL ((void *)0)

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "../include/cast/token.h"


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