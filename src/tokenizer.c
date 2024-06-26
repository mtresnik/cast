#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "../include/cast/tokenizer.h"
#include "../include/cast/util.h"
#include "../include/cast/functions.h"
#include "../include/cast/variables.h"

int compareTokensByStartIndex(const void *a, const void *b){
    const struct Token *tokenA = (const struct Token *) a;
    const struct Token *tokenB = (const struct Token *) b;
    if (tokenA->startIndex < tokenB->startIndex) {
        return -1;
    } else if (tokenA->startIndex > tokenB->startIndex) {
        return 1;
    } else {
        return 0;
    }
}

void sortTokenArrayByStartIndex(struct TokenArray array){
    qsort(array.values, array.numValues, sizeof(struct Token), compareTokensByStartIndex);
}

int indexProcessedToken(int i, struct TokenArray tokenArray) {
    for (int j = 0; j < tokenArray.numValues; j++) {
        if (tokenArray.values[j].startIndex <= i && i <= tokenArray.values[j].endIndex) {
            return 1;
        }
    }
    return 0;
}

char *preProcess(char *param) {
    return removeChar(param, ' ');
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
struct TokenArray tokenizeNumbers(char *inputString) {
    int length = strlen(inputString);
    struct TokenArray retArray = nullTokenArray();
    int count = 0;
    char* accumulate = "";
    for (int i = 0; i < length; ++i) {
        char v = inputString[i];
        if (strchr("0123456789.", v) == NULL) {
            int accumulateLen = strlen(accumulate);
            if (accumulateLen != 0){
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
                retArray = appendTokenArray(retArray, number);
                count++;
                accumulate = "";
            }
        } else {
            char* newAccumulate = appendChar(accumulate, v);
            accumulate = newAccumulate;
        }
    }
    int accumulateLen = strlen(accumulate);
    if (accumulateLen != 0) {
        auto lengthInput = strlen(inputString);
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
        retArray = appendTokenArray(retArray, number);
        free(accumulate);
    }
    return retArray;
}
#pragma clang diagnostic pop

int isOperator(char c) {
    if (strchr("+-*/^,", c) != NULL) {
        return 1;
    }
    return 0;
}

struct TokenArray tokenizeOperators(struct TokenArray array, char *inputString) {
    struct TokenArray retArray = array;
    for (int i = 0; i < strlen(inputString); ++i) {
        if (indexProcessedToken(i, retArray) == 0){
            if (isOperator(inputString[i])) {
                int start = i;
                int end = i;
                char *representation = malloc(2 * sizeof(char));
                representation[0] = inputString[i];
                representation[1] = '\0';
                struct Token operator = {
                        start,
                        end,
                        OPERATOR_TOKEN,
                        representation
                };
                retArray = appendTokenArray(retArray, operator);
            }
        }
    }
    return retArray;
}

struct TokenArray tokenizeParentheses(struct TokenArray array, char *inputString) {
    struct TokenArray retArray = array;
    for (int i = 0; i < strlen(inputString); ++i) {
        if (indexProcessedToken(i, retArray) == 0) {
            if (inputString[i] == '(') {
                int start = i;
                int end = i;
                struct Token openParenthesis;
                openParenthesis.startIndex = start;
                openParenthesis.endIndex = end;
                openParenthesis.type = OPEN_PARENTHESIS_TOKEN;
                openParenthesis.representation = "(";
                retArray = appendTokenArray(retArray, openParenthesis);
            } else if (inputString[i] == ')') {
                int start = i;
                int end = i;
                struct Token closeParenthesis;
                closeParenthesis.startIndex = start;
                closeParenthesis.endIndex = end;
                closeParenthesis.type = CLOSE_PARENTHESIS_TOKEN;
                closeParenthesis.representation = ")";
                retArray = appendTokenArray(retArray, closeParenthesis);
            }
        }
    }
    return retArray;
}

struct TokenArray tokenizeText(struct TokenArray array, char *inputString) {
    struct TokenArray retArray = array;
    char* accumulate = "";
    for (int i = 0; i < strlen(inputString); ++i) {
        char v = inputString[i];
        if (indexProcessedToken(i, retArray)) {
            size_t lengthAccumulate = strlen(accumulate);
            if (lengthAccumulate > 0) {
                int start = i - lengthAccumulate;
                int end = i - 1;
                char* representation = cloneString(accumulate);
                struct Token text = {
                    start,
                    end,
                    TEXT_TOKEN,
                    representation
                };
                retArray = appendTokenArray(retArray, text);
            }
            accumulate = "";
        } else {
            char* newAccumulate = appendChar(accumulate, v);
            accumulate = newAccumulate;
        }
    }
    size_t lengthAccumulate = strlen(accumulate);
    if (lengthAccumulate > 0) {
        int inputStringLength = strlen(inputString);
        int start = inputStringLength - lengthAccumulate;
        int end = inputStringLength - 1;
        char* representation = cloneString(accumulate);
        struct Token text = {
                start,
                end,
                TEXT_TOKEN,
                representation
        };
        retArray = appendTokenArray(retArray, text);
        free(accumulate);
    }
    sortTokenArrayByStartIndex(retArray);
    return retArray;
}

struct TokenArray tokenizeFunctions(struct TokenArray array) {
    struct TokenArray retArray = nullTokenArray();
    struct StringArray reserved = getFunctionEntryKeys();
    for (int i = 0; i < array.numValues; ++i) {
        struct Token curr = array.values[i];
        if (curr.type != TEXT_TOKEN) {
            retArray = appendTokenArray(retArray, curr);
        } else {
            if (i < array.numValues - 1 && array.values[i + 1].type == OPEN_PARENTHESIS_TOKEN) {
                char* innerFunc = "";
                int foundInner = 0;
                char* representation = curr.representation;
                for (int j = 0; j < reserved.numStrings; ++j) {
                    char* key = reserved.strings[j];
                    if (strlen(key) <= strlen(representation) && stringEndsWith(representation, key)){
                        innerFunc = key;
                        foundInner = 1;
                        break;
                    }
                }
                if (foundInner) {
                    int endIndex = lastIndexOf(representation, innerFunc);
                    if (endIndex != 0) {
                        char* newRep = substring(representation, 0, endIndex);
                        struct Token rem = nullIndex(TEXT_TOKEN, newRep);
                        retArray = appendTokenArray(retArray, rem);
                    }
                    struct Token function = nullIndex(FUNCTION_TOKEN, innerFunc);
                    retArray = appendTokenArray(retArray, function);
                } else {
                    struct Token rem = convert(curr, TEXT_TOKEN);
                    retArray = appendTokenArray(retArray, rem);
                }
            } else {
                struct Token rem = convert(curr, TEXT_TOKEN);
                retArray = appendTokenArray(retArray, rem);
            }
        }
    }
    return retArray;
}

struct TokenArray maxVariablesInString(char* inputString) {
    struct TokenArray retArray = nullTokenArray();
    int maxCount = -1;
    char* maxVar = "";
    struct StringArray reserved = getVariableEntryKeys();
    for (int i = 0; i < reserved.numStrings; ++i) {
        char* key = reserved.strings[i];
        int keyLength = strlen(key);
        if (stringContains(inputString, key) && (keyLength > maxCount)) {
            maxVar = cloneString(key);
            maxCount = strlen(maxVar);
        }
    }
    struct StringArray remainingStrings = findRemainingStrings(inputString, maxVar);
    if (maxCount == -1 || remainingStrings.numStrings == 0) {
        retArray = appendTokenArray(retArray, nullIndex(VARIABLE_TOKEN, inputString));
        return retArray;
    }
    if (remainingStrings.numStrings == 1) {
        // Right case
        if (stringStartsWith(inputString, maxVar)) {
            retArray = appendTokenArray(retArray, nullIndex(VARIABLE_TOKEN, maxVar));
            retArray = concatTokenArrays(retArray, maxVariablesInString(remainingStrings.strings[0]));
            return retArray;
        }
        // Left case
        struct TokenArray leftHandSide = maxVariablesInString(remainingStrings.strings[0]);
        retArray = concatTokenArrays(retArray, leftHandSide);
        retArray = appendTokenArray(retArray, nullIndex(VARIABLE_TOKEN, maxVar));
        return retArray;
    }
    // Middle case
    if (remainingStrings.numStrings == 2) {
        struct TokenArray leftHandSide = maxVariablesInString(remainingStrings.strings[0]);
        struct TokenArray rightHandSide = maxVariablesInString(remainingStrings.strings[1]);
        retArray = concatTokenArrays(retArray, leftHandSide);
        retArray = appendTokenArray(retArray, nullIndex(VARIABLE_TOKEN, maxVar));
        retArray = concatTokenArrays(retArray, rightHandSide);
        return retArray;
    }
    retArray = appendTokenArray(retArray, nullIndex(VARIABLE_TOKEN, inputString));
    return retArray;
}

struct TokenArray tokenizeVariables(struct TokenArray array) {
    struct TokenArray retArray = nullTokenArray();
    for (int i = 0; i < array.numValues; ++i) {
        struct Token curr = array.values[i];
        if (curr.type != TEXT_TOKEN) {
            retArray = appendTokenArray(retArray, curr);
        } else {
            retArray = concatTokenArrays(retArray, maxVariablesInString(curr.representation));
        }
    }
    return retArray;
}

struct TokenArray justifyMultiplication(struct TokenArray array) {
    struct TokenArray retArray = nullTokenArray();
    for (int i = 0; i < array.numValues; ++i) {
        struct Token curr = array.values[i];
        retArray = appendTokenArray(retArray, curr);
        if ((curr.type == NUMBER_TOKEN || curr.type == VARIABLE_TOKEN || curr.type == CLOSE_PARENTHESIS_TOKEN)
            && i < array.numValues - 1 &&
            array.values[i + 1].type != OPERATOR_TOKEN &&
            array.values[i + 1].type != CLOSE_PARENTHESIS_TOKEN) {
            char* representation = "*";
            retArray = appendTokenArray(retArray, nullIndex(OPERATOR_TOKEN, representation));
        }
    }
    return retArray;
}

struct TokenArray collapseSigns(struct TokenArray array) {
    struct TokenArray retArray = nullTokenArray();
    int i = 0;
    while (i < array.numValues) {
        struct Token curr = array.values[i];
        char* currRepresentation = curr.representation;
        char* nextRepresentation = "";
        if (i < array.numValues - 1) {
            nextRepresentation = array.values[i + 1].representation;
        }
        if (curr.type == OPERATOR_TOKEN &&
            i < array.numValues - 1 &&
            array.values[i + 1].type == OPERATOR_TOKEN &&
            (stringEquals(currRepresentation, "+") || stringEquals(currRepresentation, "-")) &&
            (stringEquals(nextRepresentation, "+") || stringEquals(nextRepresentation, "-"))){
            char* representation = "+";
            if (stringEquals(currRepresentation, nextRepresentation) == 0) {
                representation = "-";
            }
            retArray = appendTokenArray(retArray, nullIndex(OPERATOR_TOKEN, representation));
            i += 2;
        } else {
            retArray = appendTokenArray(retArray, curr);
            i++;
        }
    }
    return retArray;
}

struct TokenArray postProcess(struct TokenArray array) {
    struct TokenArray currArray = justifyMultiplication(array);
    struct TokenArray collapsed = collapseSigns(currArray);
    while (currArray.numValues != collapsed.numValues) {
        currArray = collapsed;
        collapsed = collapseSigns(currArray);
    }

    return currArray;
}

struct TokenArray tokenize(char* inputStringParam) {
    char* inputString = preProcess(inputStringParam);
    struct TokenArray tokenArray = tokenizeNumbers(inputString);
    tokenArray = tokenizeOperators(tokenArray, inputString);
    tokenArray = tokenizeParentheses(tokenArray, inputString);
    tokenArray = tokenizeText(tokenArray, inputString);
    tokenArray = tokenizeFunctions(tokenArray);
    tokenArray = tokenizeVariables(tokenArray);
    tokenArray = postProcess(tokenArray);
    return tokenArray;
}
