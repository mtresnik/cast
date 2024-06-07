#include <stdio.h>
#include <malloc.h>
#include <tgmath.h>
#include <minmax.h>
#include <memory.h>
#include <string.h>
#include <complex.h>
#include "../include/cast/parser.h"
#include "../include/cast/tokenizer.h"
#include "../include/cast/functions.h"

#define NULL_INTERMEDIATE (-1)
#define NUMBER_INTERMEDIATE (0)
#define VARIABLE_INTERMEDIATE (1)
#define ADDITION_INTERMEDIATE (2)
#define SUBTRACTION_INTERMEDIATE (3)
#define DIVISION_INTERMEDIATE (4)
#define IDENTITY_INTERMEDIATE (5)
#define NEGATION_INTERMEDIATE (6)
#define MULTIPLICATION_INTERMEDIATE (7)
#define PARENTHESES_INTERMEDIATE (8)
#define POWER_INTERMEDIATE (9)
#define FUNCTION_INTERMEDIATE (10)

#define NUMBER_TOKEN_SET (0)
#define PARENTHESES_TOKEN_SET (2)
#define FUNCTION_TOKEN_SET (5)
#define VARIABLE_TOKEN_SET (6)

int equalsTokenSet(struct TokenSet one, struct TokenSet other) {
    if (one.startIndex != other.startIndex || one.endIndex != other.endIndex) {
        return false;
    }
    if (one.type != other.type) {
        return false;
    }
    if (one.tokens.numValues != other.tokens.numValues) {
        return false;
    }
    if (one.representation == NULL && other.representation != NULL || one.representation != NULL && other.representation == NULL) {
        return false;
    }
    if (one.representation == NULL && other.representation == NULL) {
        return true;
    }
    return strcmp(one.representation, other.representation) == 0;
}

int equalsIntermediate(struct Intermediate one, struct Intermediate other) {
    if (one.startIndex != other.startIndex || one.endIndex != other.endIndex) {
        return false;
    }
    if (one.type != other.type) {
        return false;
    }
    if (one.numInner != other.numInner) {
        return false;
    }
    return true;
}

void printIntermediate(struct Intermediate intermediate) {
    printf("[Intermediate [%d,%d] : %d]\n", intermediate.startIndex, intermediate.endIndex, intermediate.type);
}

void printIntermediateArray(struct IntermediateArray array) {
    printf("Intermediate numInner:%d\n",array.numInner);
    for (int i = 0; i < array.numInner; ++i) {
        printIntermediate(array.inner[i]);
    }
}

struct Intermediate nullIntermediate() {
    struct Intermediate retStruct;
    retStruct.startIndex = -1;
    retStruct.endIndex = -1;
    retStruct.type = NULL_INTERMEDIATE;
    retStruct.tokens = nullTokenArray();
    retStruct.name = NULL;
    retStruct.numInner = 0;
    retStruct.inner = NULL;
    return retStruct;
}

struct IntermediateArray nullIntermediateArray() {
    struct IntermediateArray result;
    result.inner = NULL;
    result.arraySize = 0;
    result.numInner = 0;
    return result;
}

int printTokenSet(struct TokenSet tokenSet) {
    printf("TokenSet\n");
    printf("\tstartIndex: %d\n", tokenSet.startIndex);
    printf("\tendIndex: %d\n", tokenSet.endIndex);
    printf("\ttype: %d\n", tokenSet.type);
    printf("\trepresentation: %s\n", tokenSet.representation);
    printf("\tnumValues: %d\n", tokenSet.tokens.numValues);
    for (int i = 0; i < tokenSet.tokens.numValues; i++) {
        printf("\tToken %d:\n", i);
        printf("\t\t  startIndex: %d\n", tokenSet.tokens.values[i].startIndex);
        printf("\t\t  endIndex: %d\n", tokenSet.tokens.values[i].endIndex);
        printf("\t\t  type: %d\n", tokenSet.tokens.values[i].type);
        printf("\t\t  representation: %s\n", tokenSet.tokens.values[i].representation);
    }
    return 0;
}

int printTokenSets(struct TokenSetArray tokenSets) {
    for (int i = 0; i < tokenSets.numValues; ++i) {
        printTokenSet(tokenSets.values[i]);
    }
    return 0;
}

struct TokenSetArray cloneTokenSetArray(struct TokenSetArray array) {
    struct TokenSetArray result;
    result.numValues = array.numValues;
    result.arraySize = array.arraySize;
    result.values = malloc(result.arraySize * sizeof(struct TokenSet));
    if (result.values == NULL) {
        fprintf(stderr, "cloneTokenSetArray: Failed to allocate memory\n");
        exit(1);
    }
    memcpy(result.values, array.values, array.numValues * sizeof(struct TokenSet));
    return result;
}

struct TokenSetArray appendTokenSetArray(struct TokenSetArray base, struct TokenSet tokenSet) {
    if (base.values == NULL || base.numValues == 0) {
        struct TokenSetArray retArray;
        retArray.numValues = 1;
        retArray.arraySize = 1;
        retArray.values = malloc(sizeof(struct TokenSet));
        retArray.values[0] = tokenSet;
        return retArray;
    }
    int expectedIndex = base.numValues;
    if (expectedIndex < base.arraySize) {
        base.values[expectedIndex] = tokenSet;
        base.numValues++;
        return base;
    }
    struct TokenSetArray retArray;
    retArray.numValues = base.numValues + 1;
    retArray.arraySize = max(retArray.numValues, retArray.numValues * 1.62);
    size_t newSize = retArray.arraySize * sizeof(struct TokenSet);
    retArray.values = realloc(base.values, newSize);
    retArray.values[base.numValues] = tokenSet;
    return retArray;
}

struct IntermediateArray appendIntermediateArray(struct IntermediateArray base, struct Intermediate intermediate) {
    if (base.inner == NULL || base.numInner == 0) {
        struct IntermediateArray retArray;
        retArray.numInner = 1;
        retArray.arraySize = 2;
        retArray.inner = malloc(retArray.arraySize * sizeof(struct Intermediate));
        if (retArray.inner == NULL) {
            fprintf(stderr, "appendIntermediateArray Memory allocation failed!\n");
            exit(1);
        }
        retArray.inner[0] = intermediate;
        return retArray;
    }
    int expectedIndex = base.numInner;
    if (expectedIndex < base.arraySize) {
        base.inner[expectedIndex] = intermediate;
        base.numInner++;
        return base;
    }
    struct IntermediateArray retArray;
    retArray.numInner = base.numInner + 1;
    retArray.arraySize = max(retArray.numInner, ceilf(retArray.numInner * 1.62f));
    size_t newSize = retArray.arraySize * sizeof(struct Intermediate);
    retArray.inner = realloc(base.inner, newSize);
    if (retArray.inner == NULL) {
        fprintf(stderr, "appendIntermediateArray Memory allocation failed! realloc\n");
        exit(1);
    }
    retArray.inner[base.numInner] = intermediate;
    return retArray;
}

struct TokenSetArray nullTokenSetArray() {
    struct TokenSetArray retStruct;
    retStruct.numValues = 0;
    retStruct.arraySize = 0;
    retStruct.values = NULL;
    return retStruct;
}

struct Intermediate singleIntermediateToken(int startIndex, int endIndex, int type, struct Token token) {
    struct Intermediate retStruct;
    retStruct.startIndex = startIndex;
    retStruct.endIndex = endIndex;
    retStruct.type = type;
    retStruct.tokens = singleTokenArray(token);
    retStruct.name = NULL;
    retStruct.numInner = 0;
    retStruct.inner = NULL;
    return retStruct;
}

struct Intermediate singleIntermediate(int startIndex, int endIndex, int type, struct Intermediate inner) {
    struct Intermediate retStruct;
    retStruct.startIndex = startIndex;
    retStruct.endIndex = endIndex;
    retStruct.type = type;
    // TODO: Free memory here
    retStruct.tokens = inner.tokens;
    retStruct.name = NULL;
    retStruct.numInner = 1;
    retStruct.inner = malloc(retStruct.numInner * sizeof(struct Intermediate));
    retStruct.inner[0] = inner;
    return retStruct;
}

struct Intermediate doubleIntermediate(int startIndex, int endIndex, int type, struct Intermediate left, struct Intermediate right) {
    struct Intermediate retStruct;
    retStruct.startIndex = startIndex;
    retStruct.endIndex = endIndex;
    retStruct.type = type;
    retStruct.tokens = concatTokenArrays(left.tokens, right.tokens);
    retStruct.name = NULL;
    retStruct.numInner = 2;
    retStruct.inner = malloc(retStruct.numInner * sizeof(struct Intermediate));
    retStruct.inner[0] = left;
    retStruct.inner[1] = right;
    return retStruct;
}

struct Intermediate INumber(int startIndex, int endIndex, struct Token token) {
    return singleIntermediateToken(startIndex, endIndex, NUMBER_INTERMEDIATE, token);
}

struct Intermediate IVariable(int startIndex, int endIndex, struct Token token) {
    return singleIntermediateToken(startIndex, endIndex, VARIABLE_INTERMEDIATE, token);
}

struct Intermediate IAddition(int startIndex, int endIndex, struct Intermediate left, struct Intermediate right) {
    return doubleIntermediate(startIndex, endIndex, ADDITION_INTERMEDIATE, left, right);
}

struct Intermediate ISubtraction(int startIndex, int endIndex, struct Intermediate left, struct Intermediate right) {
    return doubleIntermediate(startIndex, endIndex, SUBTRACTION_INTERMEDIATE, left, right);
}

struct Intermediate IDivision(int startIndex, int endIndex, struct Intermediate left, struct Intermediate right) {
    return doubleIntermediate(startIndex, endIndex, DIVISION_INTERMEDIATE, left, right);
}

struct Intermediate IIdentity(int startIndex, int endIndex, struct Intermediate inner) {
    return singleIntermediate(startIndex, endIndex, IDENTITY_INTERMEDIATE, inner);
}

struct Intermediate INegation(int startIndex, int endIndex, struct Intermediate inner) {
    return singleIntermediate(startIndex, endIndex, NEGATION_INTERMEDIATE, inner);
}

struct Intermediate IMultiplication(int startIndex, int endIndex, struct Intermediate left, struct Intermediate right) {
    return doubleIntermediate(startIndex, endIndex, MULTIPLICATION_INTERMEDIATE, left, right);
}

struct Intermediate IParentheses(int startIndex, int endIndex, struct Intermediate inner) {
    struct Intermediate ret = singleIntermediate(startIndex, endIndex, PARENTHESES_INTERMEDIATE, inner);
    return ret;
}

struct Intermediate IPower(int startIndex, int endIndex, struct Intermediate left, struct Intermediate right) {
    return doubleIntermediate(startIndex, endIndex, POWER_INTERMEDIATE, left, right);
}

struct Intermediate IFunction(int startIndex, int endIndex, char* name, struct IntermediateArray array, struct TokenArray tokensParam) {
    struct Intermediate retStruct;
    retStruct.startIndex = startIndex;
    retStruct.endIndex = endIndex;
    retStruct.type = FUNCTION_INTERMEDIATE;
    struct TokenArray tokens = nullTokenArray();
    for (int i = 0; i < array.numInner; ++i) {
        struct Intermediate curr = array.inner[i];
        // TODO: Free memory here
        tokens = concatTokenArrays(tokens, curr.tokens);
    }
    retStruct.tokens = tokens;
    retStruct.name = name;
    retStruct.numInner = array.numInner;
    retStruct.inner = array.inner;
    return retStruct;
}

struct Operation compile(struct Intermediate intermediate);

struct OperationArray compileArray(struct IntermediateArray intermediateArray){
    struct OperationArray retArray;
    retArray.numValues = intermediateArray.numInner;
    retArray.values = malloc(retArray.numValues * sizeof(struct Operation));
    for (int i = 0; i < retArray.numValues; ++i) {
        retArray.values[i] = malloc(sizeof(struct Operation));
        *retArray.values[i] = compile(intermediateArray.inner[i]);
    }
    return retArray;
}

struct Operation compile(struct Intermediate intermediate) {
    if (intermediate.tokens.numValues == 0 || intermediate.tokens.values == NULL) {
        return nullOperation(0, NULL);
    }
    switch (intermediate.type) {
        case NUMBER_INTERMEDIATE:
            double innerValue = strtod(intermediate.tokens.values[0].representation, NULL);
            double complex* complexValue = malloc(sizeof(double complex));
            complexValue[0] = innerValue;
            return Constant(complexValue);
        case VARIABLE_INTERMEDIATE:
            char** varString = malloc(sizeof(char**));
            *varString = intermediate.tokens.values[0].representation;
            return Variable(varString);
        case ADDITION_INTERMEDIATE:
            // TODO Free memory
            {
                struct Operation *leftCompiledPtr = malloc(sizeof(struct Operation));
                leftCompiledPtr[0] = compile(intermediate.inner[0]);
                struct Operation *rightCompiledPtr = malloc(sizeof(struct Operation));
                rightCompiledPtr[0] = compile(intermediate.inner[1]);
                return Addition(leftCompiledPtr, rightCompiledPtr);
            }
        case SUBTRACTION_INTERMEDIATE:
            // TODO Free memory
            {
                struct Operation *leftCompiledPtr = malloc(sizeof(struct Operation ));
                leftCompiledPtr[0] = compile(intermediate.inner[0]);
                struct Operation *rightCompiledPtr = malloc(sizeof(struct Operation ));
                rightCompiledPtr[0] = compile(intermediate.inner[1]);
                return Subtraction(leftCompiledPtr, rightCompiledPtr);
            }
        case DIVISION_INTERMEDIATE:
            // TODO Free memory
            {
                struct Operation *leftCompiledPtr = malloc(sizeof(struct Operation ));
                leftCompiledPtr[0] = compile(intermediate.inner[0]);
                struct Operation *rightCompiledPtr = malloc(sizeof(struct Operation ));
                rightCompiledPtr[0] = compile(intermediate.inner[1]);
                return Division(leftCompiledPtr, rightCompiledPtr);
            }
        case IDENTITY_INTERMEDIATE:
            return compile(intermediate.inner[0]);
        case NEGATION_INTERMEDIATE:
            // TODO Free memory
            {
                struct Operation *innerPtr = malloc(sizeof(struct Operation));
                *innerPtr = compile(intermediate.inner[0]);
                return Negation(innerPtr);
            }
        case MULTIPLICATION_INTERMEDIATE:
            // TODO Free memory
            {
                struct Operation *leftCompiledPtr = malloc(sizeof(struct Operation ));
                leftCompiledPtr[0] = compile(intermediate.inner[0]);
                struct Operation *rightCompiledPtr = malloc(sizeof(struct Operation ));
                rightCompiledPtr[0] = compile(intermediate.inner[1]);
                return Multiplication(leftCompiledPtr, rightCompiledPtr);
            }
        case PARENTHESES_INTERMEDIATE:
            // TODO Free memory
            {
                struct Operation *innerPtr = malloc(sizeof(struct Operation));
                *innerPtr = compile(intermediate.inner[0]);
                return Parentheses(innerPtr);
            }
        case POWER_INTERMEDIATE:
            // TODO Free memory
            {
                struct Operation *leftCompiledPtr = malloc(sizeof(struct Operation ));
                leftCompiledPtr[0] = compile(intermediate.inner[0]);
                struct Operation *rightCompiledPtr = malloc(sizeof(struct Operation ));
                rightCompiledPtr[0] = compile(intermediate.inner[1]);
                return Power(leftCompiledPtr, rightCompiledPtr);
            }
        case FUNCTION_INTERMEDIATE:
            {
                struct IntermediateArray innerArray;
                innerArray.numInner = intermediate.numInner;
                innerArray.arraySize = intermediate.numInner;
                innerArray.inner = intermediate.inner;
                struct OperationArray innerCompiled = compileArray(innerArray);
                struct Operation (*value) (struct OperationArray operations) = getFunctionEntryValue(intermediate.name);
                struct Operation retValue = value(innerCompiled);
                return retValue;
            }
        default:
            return nullOperation(0, NULL);
    }
    return nullOperation(0, NULL);
}


struct TokenSetArray generateParentheses(struct TokenArray inputList) {
    struct TokenSetArray retList = nullTokenSetArray();
    struct TokenArray inner = nullTokenArray();
    int balance = 0;
    int startIndex = -1;
    for (int i = 0; i < inputList.numValues; ++i) {
        struct Token token = inputList.values[i];
        if (token.type == OPEN_PARENTHESIS_TOKEN) {
            balance--;
        } else if (token.type == CLOSE_PARENTHESIS_TOKEN) {
            balance++;
        }
        if (balance == -1 && token.type == OPEN_PARENTHESIS_TOKEN) {
            startIndex = i;
        }
        if (balance == 0 && token.type == CLOSE_PARENTHESIS_TOKEN) {
            for (int j = startIndex + 1; j < i; ++j) {
                inner = appendTokenArray(inner, inputList.values[j]);
            }
            struct TokenSet next = {
                    startIndex,
                    i,
                    PARENTHESES_TOKEN_SET,
                    inner,
                    NULL
            };
            retList = appendTokenSetArray(retList, next);
            startIndex = -1;
            inner = nullTokenArray();
        }
    }
    return retList;
}

bool indexProcessedTokenSet(int i, struct TokenSetArray tokenList) {
    for (int j = 0; j < tokenList.numValues; ++j) {
        struct TokenSet t = tokenList.values[j];
        if (i >= t.startIndex && i <= t.endIndex) {
            return true;
        }
    }
    return false;
}

struct TokenSetArray removeTokenSetArrayAtIndex(struct TokenSetArray array, int index) {
    if (array.values == NULL || array.numValues == 0) {
        return array;
    }
    if (index < 0 || index >= array.numValues) {
        return array;
    }
    struct TokenSetArray retArray;
    retArray.numValues = array.numValues - 1;
    retArray.arraySize = array.arraySize - 1;
    retArray.values = malloc(retArray.arraySize * sizeof(struct TokenSet));
    int j = 0;
    for (int i = 0; i < array.numValues; ++i) {
        if (i != index) {
            retArray.values[j] = array.values[i];
            j++;
        }
    }
    return retArray;
}


struct TokenSetArray generateFunctions(struct TokenSetArray current, struct TokenArray inputList) {
    struct TokenSetArray clone = cloneTokenSetArray(current);
    struct TokenSetArray retList = cloneTokenSetArray(current);
    for (int i = 0; i < inputList.numValues; ++i) {
        struct Token token = inputList.values[i];
        if (!indexProcessedTokenSet(i, current)) {
            if (token.type == FUNCTION_TOKEN) {
                int foundIndex = -1;
                int expectedIndex = i + 1;
                for (int j = 0; j < clone.numValues; ++j) {
                    struct TokenSet set = clone.values[j];
                    if (set.type == PARENTHESES_TOKEN_SET) {
                        if (set.startIndex == expectedIndex) {
                            foundIndex = j;
                            break;
                        }
                    }
                }
                if (foundIndex != -1) {
                    struct TokenSet found = clone.values[foundIndex];
                    clone = removeTokenSetArrayAtIndex(clone, foundIndex);
                    for (int j = 0; j < retList.numValues; ++j) {
                        if (equalsTokenSet(found, retList.values[j])) {
                            foundIndex = j;
                            break;
                        }
                    }
                    retList = removeTokenSetArrayAtIndex(retList, foundIndex);
                    struct TokenSet fn;
                    fn.startIndex = i;
                    fn.endIndex = found.endIndex;
                    fn.type = FUNCTION_TOKEN_SET;
                    fn.tokens = found.tokens;
                    fn.representation = found.representation;
                    retList = appendTokenSetArray(retList, fn);
                }
            }
        }
    }
    return retList;
}

int compareTokenSetArrayByStartIndex(const void *a, const void *b){
    const struct TokenSet *tokenA = (const struct TokenSet *) a;
    const struct TokenSet *tokenB = (const struct TokenSet *) b;
    if (tokenA->startIndex < tokenB->startIndex) {
        return -1;
    } else if (tokenA->startIndex > tokenB->startIndex) {
        return 1;
    } else {
        return 0;
    }
}

void sortTokenSetArrayByStartIndex(struct TokenSetArray array){
    qsort(array.values, array.numValues, sizeof(struct TokenSet), compareTokenSetArrayByStartIndex);
}

struct TokenSetArray generateVariables(struct TokenSetArray current, struct TokenArray inputList) {
    struct TokenSetArray retList = current;
    for (int i = 0; i < inputList.numValues; ++i) {
        struct Token token = inputList.values[i];
        if (!indexProcessedTokenSet(i, retList)) {
            if (token.type == VARIABLE_TOKEN) {
                char* representation = malloc(strlen(token.representation) * sizeof(char));
                strcpy(representation, token.representation);
                struct TokenSet variable = {
                        i,
                        i,
                        VARIABLE_TOKEN_SET,
                        singleTokenArray(token),
                        representation
                };
                retList = appendTokenSetArray(retList, variable);
            }
        }
    }
    sortTokenSetArrayByStartIndex(retList);
    return retList;
}

struct TokenSetArray generateNumbers(struct TokenSetArray current, struct TokenArray inputList) {
    struct TokenSetArray retList = current;
    for (int i = 0; i < inputList.numValues; ++i) {
        struct Token token = inputList.values[i];
        if (!indexProcessedTokenSet(i, current)) {
            if (token.type == NUMBER_TOKEN) {
                struct TokenSet number = {
                        i,
                        i,
                        NUMBER_TOKEN_SET,
                        singleTokenArray(token)
                };
                retList = appendTokenSetArray(retList, number);
            }
        }
    }
    sortTokenSetArrayByStartIndex(retList);
    return retList;
}

struct TokenSetArray generateTokenSets(struct TokenArray inputList) {
    struct TokenSetArray tokenSets = generateParentheses(inputList);
    tokenSets = generateFunctions(tokenSets, inputList);
    tokenSets = generateVariables(tokenSets, inputList);
    return generateNumbers(tokenSets, inputList);
}

struct Intermediate generateIntermediate(struct TokenArray array);

struct IntermediateArray generateMultipleIntermediates(struct TokenArray array);

struct IntermediateArray generateIntermediates(struct TokenSetArray current) {
    struct IntermediateArray retList = nullIntermediateArray();
    for (int i = 0; i < current.numValues; ++i) {
        struct TokenSet set = current.values[i];
        switch (set.type) {
            case NUMBER_TOKEN_SET:
                struct Intermediate number = INumber(set.startIndex, set.endIndex, set.tokens.values[0]);
                retList = appendIntermediateArray(retList, number);
                break;
            case VARIABLE_TOKEN_SET:
                struct Intermediate variable = IVariable(set.startIndex, set.endIndex, set.tokens.values[0]);
                retList = appendIntermediateArray(retList, variable);
                break;
            case PARENTHESES_TOKEN_SET: {
                struct TokenArray cloned = cloneTokenArray(set.tokens);
                struct Intermediate inner = generateIntermediate(cloned);
                struct Intermediate parentheses = IParentheses(set.startIndex, set.endIndex, inner);
                retList = appendIntermediateArray(retList, parentheses);
                break;
            }
            case FUNCTION_TOKEN_SET:
                struct IntermediateArray inner = generateMultipleIntermediates(set.tokens);
                struct Intermediate function = IFunction(set.startIndex, set.endIndex, set.representation, inner, set.tokens);
                retList = appendIntermediateArray(retList, function);
                break;
            default:
                break;
        }
    }
    return retList;
}

struct Intermediate getLeftIntermediate(int i, struct IntermediateArray intermediateList) {
    if (intermediateList.inner == NULL) {
        return nullIntermediate();
    }
    for (int j = 0; j < intermediateList.numInner; ++j) {
        struct Intermediate intermediate = intermediateList.inner[j];
        if (((i - 1) >= intermediate.startIndex) &&  ((i - 1) <= intermediate.endIndex)) {
            return intermediate;
        }
    }
    return nullIntermediate();
}

struct Intermediate getRightIntermediate(int i, struct IntermediateArray intermediateList) {
    if (intermediateList.inner == NULL) {
        return nullIntermediate();
    }
    for (int j = 0; j < intermediateList.numInner; j++) {
        struct Intermediate intermediate = intermediateList.inner[j];
        if (((i + 1) >= intermediate.startIndex) && ((i + 1) <= intermediate.endIndex)) {
            return intermediate;
        }
    }
    return nullIntermediate();
}

struct IntermediateArray removeIntermediateArrayElement(struct IntermediateArray intermediates, struct Intermediate other) {
    struct IntermediateArray result;
    result.numInner = intermediates.numInner;
    result.arraySize = intermediates.arraySize;
    result.inner = malloc(result.arraySize * sizeof(struct Intermediate));
    int j = 0;
    for (int i = 0; i < intermediates.numInner; ++i) {
        struct Intermediate intermediate = intermediates.inner[i];
        if (!equalsIntermediate(intermediate, other)) {
            result.inner[j] = intermediate;
            j++;
        }
    }
    result.numInner = j;
    return result;
}

int compareIntermediateArrayByStartIndex(const void *a, const void *b){
    const struct Intermediate *intermediate1 = (const struct Intermediate *) a;
    const struct Intermediate *intermediate2 = (const struct Intermediate *) b;
    if (intermediate1->startIndex < intermediate2->startIndex) {
        return -1;
    } else if (intermediate1->startIndex > intermediate2->startIndex) {
        return 1;
    } else {
        return 0;
    }
}

void sortIntermediateArrayByStartIndex(struct IntermediateArray intermediates) {
    qsort(intermediates.inner, intermediates.numInner, sizeof(struct Intermediate), compareIntermediateArrayByStartIndex);
}

bool indexProcessedOperation(int i, struct IntermediateArray intermediateList) {
    for (int j = 0; j < intermediateList.numInner; ++j) {
        struct Intermediate t = intermediateList.inner[j];
        if (i >= t.startIndex && i <= t.endIndex) {
            return true;
        }
    }
    return false;
}

struct IntermediateArray generateIdentities(struct IntermediateArray current, struct TokenArray inputList) {
    struct IntermediateArray clone = current;
    for (int i = 0; i < inputList.numValues; i++) {
        struct Token token = inputList.values[i];
        if (!indexProcessedOperation(i, clone)){
            if (token.type == OPERATOR_TOKEN) {
                if (strcmp(token.representation, "+") == 0) {
                    struct Intermediate left = getLeftIntermediate(i, clone);
                    struct Intermediate right = getRightIntermediate(i, clone);
                    if (left.type == NULL_INTERMEDIATE && right.type != NULL_INTERMEDIATE) {
                        clone = removeIntermediateArrayElement(clone, right);
                        struct Intermediate identity = IIdentity(i, right.endIndex, right);
                        clone = appendIntermediateArray(clone, identity);
                    }
                } else if (strcmp(token.representation, "-") == 0) {
                    struct Intermediate left = getLeftIntermediate(i, clone);
                    struct Intermediate right = getRightIntermediate(i, clone);
                    if (left.type == NULL_INTERMEDIATE && right.type != NULL_INTERMEDIATE) {
                        clone = removeIntermediateArrayElement(clone, right);
                        struct Intermediate negation = INegation(i, right.endIndex, right);
                        clone = appendIntermediateArray(clone, negation);
                    }
                }
            }
        }
    }
    struct IntermediateArray retList = clone;
    sortIntermediateArrayByStartIndex(retList);
    return retList;
}

struct IntermediateArray generatePowers(struct IntermediateArray current, struct TokenArray inputList) {
    struct IntermediateArray clone = current;
    for (int i = 0; i < inputList.numValues; ++i) {
        struct Token token = inputList.values[i];
        if (!indexProcessedOperation(i, clone)){
            if (token.type == OPERATOR_TOKEN) {
                if (strcmp(token.representation, "^") == 0) {
                    struct Intermediate left = getLeftIntermediate(i, clone);
                    struct Intermediate right = getRightIntermediate(i, clone);
                    if (left.type != NULL_INTERMEDIATE && right.type != NULL_INTERMEDIATE) {
                        clone = removeIntermediateArrayElement(clone, left);
                        clone = removeIntermediateArrayElement(clone, right);
                        struct Intermediate power = IPower(left.startIndex, right.startIndex, left, right);
                        clone = appendIntermediateArray(clone, power);
                    }
                }
            }
        }
    }
    struct IntermediateArray retList = clone;
    sortIntermediateArrayByStartIndex(retList);
    return retList;
}

struct IntermediateArray generateMultiplicationAndDivision(struct IntermediateArray current, struct TokenArray inputList) {
    struct IntermediateArray clone = current;
    for (int i = 0; i < inputList.numValues; ++i) {
        struct Token token = inputList.values[i];
        if (!indexProcessedOperation(i, clone)){
            if (token.type == OPERATOR_TOKEN) {
                if (strcmp(token.representation, "*") == 0) {
                    struct Intermediate left = getLeftIntermediate(i, clone);
                    struct Intermediate right = getRightIntermediate(i, clone);
                    if (left.type != NULL_INTERMEDIATE && right.type != NULL_INTERMEDIATE) {
                        clone = removeIntermediateArrayElement(clone, left);
                        clone = removeIntermediateArrayElement(clone, right);
                        struct Intermediate multiplication = IMultiplication(left.startIndex, right.endIndex, left, right);
                        clone = appendIntermediateArray(clone, multiplication);
                    }
                } else if (strcmp(token.representation, "/") == 0) {
                    struct Intermediate left = getLeftIntermediate(i, clone);
                    struct Intermediate right = getRightIntermediate(i, clone);
                    if (left.type != NULL_INTERMEDIATE && right.type != NULL_INTERMEDIATE) {
                        clone = removeIntermediateArrayElement(clone, left);
                        clone = removeIntermediateArrayElement(clone, right);
                        struct Intermediate division = IDivision(left.startIndex, right.endIndex, left, right);
                        clone = appendIntermediateArray(clone, division);
                    }
                }
            }
        }
    }
    struct IntermediateArray retList = clone;
    sortIntermediateArrayByStartIndex(retList);
    return retList;
}

struct IntermediateArray generateAdditionAndSubtraction(struct IntermediateArray current, struct TokenArray inputList) {
    struct IntermediateArray clone = current;
    for (int i = 0; i < inputList.numValues; ++i) {
        struct Token token = inputList.values[i];
        if (!indexProcessedOperation(i, clone)){
            if (token.type == OPERATOR_TOKEN) {
                if (strcmp(token.representation, "+") == 0) {
                    struct Intermediate left = getLeftIntermediate(i, clone);
                    struct Intermediate right = getRightIntermediate(i, clone);
                    if (left.type != NULL_INTERMEDIATE && right.type != NULL_INTERMEDIATE) {
                        clone = removeIntermediateArrayElement(clone, left);
                        clone = removeIntermediateArrayElement(clone, right);
                        struct Intermediate addition = IAddition(left.startIndex, right.endIndex, left, right);
                        clone = appendIntermediateArray(clone, addition);
                    }
                } else if (strcmp(token.representation, "-") == 0) {
                    struct Intermediate left = getLeftIntermediate(i, clone);
                    struct Intermediate right = getRightIntermediate(i, clone);
                    if (left.type != NULL_INTERMEDIATE && right.type != NULL_INTERMEDIATE) {
                        clone = removeIntermediateArrayElement(clone, left);
                        clone = removeIntermediateArrayElement(clone, right);
                        struct Intermediate subtraction = ISubtraction(left.startIndex, right.endIndex, left, right);
                        clone = appendIntermediateArray(clone, subtraction);
                    }
                }
            }
        }
    }
    struct IntermediateArray retList = clone;
    sortIntermediateArrayByStartIndex(retList);
    return retList;
}

struct IntermediateArray generateOperators(struct IntermediateArray current, struct TokenArray inputList) {
    struct IntermediateArray intermediates = generateIdentities(current, inputList);
    intermediates = generatePowers(intermediates, inputList);
    intermediates = generateMultiplicationAndDivision(intermediates, inputList);
    intermediates = generateAdditionAndSubtraction(intermediates, inputList);
    return intermediates;
}

struct IntermediateArray generateMultipleIntermediates(struct TokenArray array) {
    struct TokenSetArray tokenSets = generateTokenSets(array);
    struct IntermediateArray intermediates = generateIntermediates(tokenSets);
    intermediates = generateOperators(intermediates, array);
    return intermediates;
}

struct Intermediate generateIntermediate(struct TokenArray array) {
    struct IntermediateArray intermediates = generateMultipleIntermediates(array);
    if (intermediates.numInner == 1) {
        struct Intermediate ret = intermediates.inner[0];
        free(intermediates.inner);
        return ret;
    }
    return nullIntermediate();
}

struct Operation parseOperation(char* inputString) {
    struct TokenArray tokenArray = tokenize(inputString);
    struct Intermediate intermediateOperation = generateIntermediate(tokenArray);
    struct Operation operation = compile(intermediateOperation);
    free(intermediateOperation.inner);
    free(tokenArray.values);
    double complex* e = malloc(sizeof(double complex));
    e[0] = cexp(1.0 + 0.0*I);
    char** eString = malloc(sizeof(char*));
    eString[0] = "e";
    operation = evaluate(operation, Variable(eString), NamedConstant(e, "e"));

    double complex* i = malloc(sizeof(double complex));
    i[0] = 0.0 + 1.0 * I;
    char** iString = malloc(sizeof(char*));
    iString[0] = "i";
    operation = evaluate(operation, Variable(iString), NamedConstant(i, "i"));
    return operation;
}