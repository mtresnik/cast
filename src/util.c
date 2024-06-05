#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "../include/cast/util.h"

struct StringArray nullStringArray() {
    struct StringArray retArray;
    retArray.numStrings = 0;
    retArray.strings = NULL;
    return retArray;
}

int printStringArray(struct StringArray array) {
    for (int i = 0; i < array.numStrings; ++i) {
        printf("%s\n", array.strings[i]);
    }
    return 0;
}


char* cloneString(char* inputString){
    size_t length = strlen(inputString);
    char*  retString = malloc(length);
    strcpy(retString, inputString);
    retString[length] = '\0';
    return retString;
}

char* appendChar(char* inputString, char c) {
    size_t length = strlen(inputString);
    char*  retString = malloc(length + 2);
    strcpy(retString, inputString);
    retString[length] = c;
    retString[length + 1] = '\0';
    return retString;
}

int countChar(char* inputString, char c){
    int count = 0;
    for(size_t i = 0; i < strlen(inputString); i++) {
        if (inputString[i] == c) {
            count++;
        }
    }
    return count;
}

char* replaceChar(char* inputString, char c, char newChar) {
    size_t inputSize = strlen(inputString);
    char* retString = malloc(inputSize * sizeof(char));
    for (size_t i = 0; i < inputSize; i++) {
        if (inputString[i] == c) {
            retString[i] = newChar;
        } else {
            retString[i] = inputString[i];
        }
    }
    retString[inputSize] = '\0';
    return retString;
}

char* removeChar(char* inputString, char c){
    size_t inputSize = strlen(inputString);
    size_t newSize = inputSize - countChar(inputString, c);
    char* retString = malloc(newSize * sizeof(char));
    retString[newSize] = '\0';
    int j = 0;
    for (int i = 0; i < inputSize; ++i) {
        if (inputString[i] != c) {
            retString[j] = inputString[i];
            j++;
        }
    }
    return retString;
}

long getHash(char* inputString){
    size_t inputSize = strlen(inputString);
    long hash = 0;
    for (size_t i = 0; i < inputSize; i++) {
        hash = 31 * hash + inputString[i];
    }
    return hash;
}

int safeMod(long one, int other) {
    int ret = one % other;
    while (ret < 0) {
        ret += other;
    }
    return ret;
}

int firstIndexOf(char* one, char* other) {
    size_t oneLength = strlen(one);
    size_t otherLength = strlen(other);
    if (oneLength < otherLength) {
        return -1;
    }
    for (int i = 0; i <= (oneLength - otherLength); i++) {
        if (strncmp(one + i, other, otherLength) == 0) {
            return i;
        }
    }
    return -1;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
int lastIndexOf(char* one, char* other) {
    size_t oneLength = strlen(one);
    size_t otherLength = strlen(other);
    if (oneLength < otherLength) {
        return -1;
    }
    for (int i = (oneLength - otherLength); i >= 0; i--) {
        if (strncmp(one + i, other, otherLength) == 0) {
            return i;
        }
    }
    return -1;
}
#pragma clang diagnostic pop

int stringStartsWith(char* one, char* other){
    return firstIndexOf(one, other) == 0;
}

int stringEndsWith(char* one, char* other) {
    size_t oneLength = strlen(one);
    size_t otherLength = strlen(other);
    if (oneLength < otherLength) {
        return 0;
    }
    return strcmp(one + (oneLength - otherLength), other) == 0;
}

int stringContains(char* one, char* other){
    return firstIndexOf(one, other) != -1;
}

int stringEquals(char* one, char* other) {
    return strcmp(one, other) == 0;
}

char* substring(const char* representation, int startIndex, int endIndex) {
    size_t length = endIndex - startIndex;
    char *retString = malloc(length * sizeof(char));
    for (int i = startIndex; i < endIndex; i++) {
        retString[i - startIndex] = representation[i];
    }
    retString[length] = '\0';
    return retString;
}

char* substringToEnd(const char* representation, int startIndex) {
    return substring(representation, startIndex, strlen(representation));
}

struct StringArray findRemainingStrings(char* test, char* key) {
    if (strlen(test) == 0 || stringContains(test, key) == false || strlen(test) == strlen(key)) {
        return nullStringArray();
    }
    int index = firstIndexOf(test, key);
    if (index == 0) {
        char* remaining = substringToEnd(test, strlen(key));
        char** strings = malloc(sizeof(char *));
        strings[0] = remaining;
        struct StringArray retArray;
        retArray.numStrings = 1;
        retArray.strings = strings;
        return retArray;
    }
    if (stringEndsWith(test, key)) {
        char* remaining = substring(test, 0, strlen(test) - strlen(key));
        char** strings = malloc(sizeof(char *));
        strings[0] = remaining;
        struct StringArray retArray;
        retArray.numStrings = 1;
        retArray.strings = strings;
        return retArray;
    } else {
        char* leftString = substring(test, 0, index);
        char *rightString = substringToEnd(test, index + strlen(key));
        char **strings = malloc(2 * sizeof(char *));
        strings[0] = leftString;
        strings[1] = rightString;
        struct StringArray retArray;
        retArray.numStrings = 2;
        retArray.strings = strings;
        return retArray;
    }
}