#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "../include/cast/tokenizer.h"
#include "../include/cast/util.h"

char *preProcess(char *param) {
    return param;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
struct TokenArray tokenizeNumbers(char *inputString) {
    struct TokenArray result;
    auto length = strlen(inputString);
    struct Token* retArray = malloc(length * sizeof(struct Token));
    int count = 0;
    char* accumulate = "";
    for (size_t i = 0; i < length; ++i) {
        char v = inputString[i];
        if (strchr("0123456789.", v) == NULL) {
            if (strlen(accumulate) != 0){
                auto accumulateLen = strlen(accumulate);
                int start = i - accumulateLen;
                int end = i - 1;
                char* representation = malloc(accumulateLen * sizeof(char));
                strcpy(representation, accumulate);
                struct Token number = {
                        start,
                        end,
                        NUMBER_TOKEN,
                        representation
                };
                retArray[count] = number;
                count++;
                free(accumulate);
                accumulate = "";
            }
        } else {
            char* newAccumulate = appendChar(accumulate, v);
            // free(accumulate);
            accumulate = newAccumulate;
        }
    }
    if (strlen(accumulate) != 0) {
        auto lengthInput = strlen(inputString);
        auto accumulateLen = strlen(accumulate);
        int start = lengthInput - accumulateLen;
        int end = lengthInput - 1;
        char* representation = malloc(accumulateLen * sizeof(char));
        strcpy(representation, accumulate);
        struct Token number = {
                start,
                end,
                NUMBER_TOKEN,
                representation
        };
        retArray[count] = number;
        count++;
        free(accumulate);
    }
    result.numValues = count;
    result.values = retArray;
    return result;
}
#pragma clang diagnostic pop

struct TokenArray tokenize(char* inputStringParam) {
    char* inputString = preProcess(inputStringParam);
    struct TokenArray tokenArray = tokenizeNumbers(inputString);
    return tokenArray;
}
