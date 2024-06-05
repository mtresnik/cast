

#ifndef CAST_UTIL_H
#define CAST_UTIL_H

struct StringArray {
    int numStrings;
    char** strings;
};

int printStringArray(struct StringArray array);

char* cloneString(char* inputString);

char* appendChar(char* inputString, char c);

char* replaceChar(char* inputString, char c, char newChar);

char* removeChar(char* inputString, char c);

long getHash(char* inputString);

int safeMod(long one, int other);

int firstIndexOf(char* one, char* other);

int lastIndexOf(char* one, char* other);

int stringStartsWith(char* one, char* other);

int stringEndsWith(char* one, char* other);

int stringContains(char* one, char* other);

int stringEquals(char* one, char* other);

char* substring(const char* representation, int startIndex, int endIndex);

struct StringArray findRemainingStrings(char* test, char* key);

#endif //CAST_UTIL_H
