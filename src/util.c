#include <string.h>
#include <malloc.h>
#include "../include/cast/util.h"


char* appendChar(char* inputString, char c) {
    size_t length = strlen(inputString);
    char*  retString = malloc(length + 2);
    strcpy(retString, inputString);
    retString[length] = c;
    retString[length + 1] = '\0';
    return retString;
}