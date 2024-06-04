

#include <stdio.h>
#include "test_util.h"
#include "../include/cast/util.h"


int testUtil() {
    char* testString = "abc";
    char* appended = appendChar(testString, '1');
    printf("appended: %s\n", appended);
}