#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#include "../include/cast/operation.h"

struct Operation nullOperation(int numValues, struct Operation **values){
    struct Operation operation;
    operation.numValues = numValues;
    operation.values = values;
    operation.type = NULL_OP;
    operation.representation = NULL;
    operation.number_representation = NULL;
    return operation;
}

struct Operation Constant(double complex* number) {
    struct Operation operation;
    operation.numValues = 0;
    operation.values = NULL;
    operation.type = CONSTANT_OPERATION;
    operation.representation = NULL;
    operation.number_representation = number;
    return operation;
}

struct Operation Variable(char** representation) {
    struct Operation operation;
    operation.numValues = 0;
    operation.values = NULL;
    operation.type = VARIABLE_OPERATION;
    operation.representation = representation;
    operation.number_representation = NULL;
    return operation;
}

struct Operation newOneOp(int type, struct Operation *one){
    struct Operation operation;
    operation.numValues = 1;
    struct Operation **values = malloc(1 * sizeof(struct Operation));
    values[0] = one;
    operation.values = values;
    operation.type = type;
    operation.representation = NULL;
    operation.number_representation = NULL;
    return operation;
}

struct Operation newTwoOp(int type, struct Operation *one, struct Operation *other){
    struct Operation operation;
    operation.numValues = 2;
    struct Operation **values = malloc(2 * sizeof(struct Operation));
    values[0] = one;
    values[1] = other;
    operation.values = values;
    operation.type = type;
    operation.representation = NULL;
    operation.number_representation = NULL;
    return operation;
}

struct Operation Abs(struct Operation *one) {
    return newOneOp(ABS_OPERATION, one);
}

struct Operation Addition(struct Operation *one, struct Operation *other) {
    return newTwoOp(ADDITION_OPERATION, one, other);
}

struct Operation ArcCos(struct Operation *one) {
    return newOneOp(ARCCOS_OPERATION, one);
}

struct Operation ArcSin(struct Operation *one) {
    return newOneOp(ARCSIN_OPERATION, one);
}

struct Operation ArcTan(struct Operation *one) {
    return newOneOp(ARCTAN_OPERATION, one);
}

struct Operation Cos(struct Operation *one) {
    return newOneOp(COS_OPERATION, one);
}

struct Operation Division(struct Operation *one, struct Operation *other) {
    return newTwoOp(DIVISION_OPERATION, one, other);
}

struct Operation Log(struct Operation *one, struct Operation *other) {
    return newTwoOp(LOG_OPERATION, one, other);
}

struct Operation Multiplication(struct Operation *one, struct Operation *other) {
    return newTwoOp(MULTIPLICATION_OPERATION, one, other);
}

struct Operation Negation(struct Operation *one) {
    return newOneOp(NEGATION_OPERATION, one);
}

struct Operation Parentheses(struct Operation *one) {
    return newOneOp(PARENTHESES_OPERATION, one);
}

struct Operation Power(struct Operation *one, struct Operation *other) {
    return newTwoOp(POWER_OPERATION, one, other);
}

struct Operation Sin(struct Operation *one) {
    return newOneOp(SIN_OPERATION, one);
}

struct Operation Subtraction(struct Operation *one, struct Operation *other) {
    return newTwoOp(SUBTRACTION_OPERATION, one, other);
}

struct Operation Tan(struct Operation *one) {
    return newOneOp(TAN_OPERATION, one);
}

int hasNestedValues(struct Operation root) {
    if (root.values == NULL) {
        return false;
    }
    for (int i = 0; i < root.numValues; ++i) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
        struct Operation element = *root.values[i];
#pragma clang diagnostic pop
        if (element.numValues > 0) {
            return true;
        }
    }
    return false;
}

struct Operation flatten(struct Operation root) {
    if (root.numValues == 0){
        return nullOperation(root.numValues, root.values);
    }
    int count = 0;
    for (int i = 0; i < root.numValues; i++) {
        struct Operation *element = root.values[i];
        if (element->numValues > 0) {
            count += element->numValues;
        } else {
            count++;
        }
    }
    struct Operation **values = malloc(count * sizeof(struct Operation));
    int outerIndex = 0;
    for (int i = 0; i < root.numValues; i++) {
        struct Operation element = *(root.values[i]);
        if (element.numValues > 0) {
            struct Operation **childValues = element.values;
            for (int j = 0; j < element.numValues; ++j) {
                values[outerIndex] = childValues[j];
                outerIndex++;
            }
        } else {
            values[outerIndex] = root.values[i];
            outerIndex++;
        }
    }
    return nullOperation(count, values);
}

struct Operation deepFlatten(struct Operation root) {
    if (root.numValues == 0) {
        struct Operation **result = malloc(sizeof(struct Operation));
        result[0] = &root;
        return nullOperation(1, result);
    } else {
        struct Operation retOperation = flatten(root);
        while (hasNestedValues(retOperation)) {
            struct Operation newArray = flatten(retOperation);
            if (retOperation.values != NULL) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "DanglingPointer"
                free(retOperation.values);
#pragma clang diagnostic pop
            }
            retOperation = newArray;
        }
        return retOperation;
    }
}


int countFlattened(struct Operation root) {
    struct Operation flattened = deepFlatten(root);
    if (flattened.values != NULL) {
        free(flattened.values);
    }
    return flattened.numValues;
}

int isConstant(struct Operation operation) {
    switch (operation.type) {
        case CONSTANT_OPERATION:
            return true;
        case VARIABLE_OPERATION:
            return false;
        default:
            for (int i = 0; i < operation.numValues; ++i) {
                if (isConstant(*(operation.values[i])) == false) {
                    return false;
                }
            }
            return true;
    }
}

char *toStringOperation(struct Operation operation) {
    if (isConstant(operation)) {
        double complex number = toNumber(operation);
        float real = crealf(number);
        float imag = cimagf(number);
        char *retString = (char *) malloc(32 * sizeof(char)); // Heap allocation, needs to be freed
        sprintf(retString, "%f + %f * i", real, imag);
        return retString;
    }
    int assumedSize = countFlattened(operation);
    char* retString = (char *) malloc(16 * assumedSize * sizeof(char));

    if (operation.type == VARIABLE_OPERATION) {
        if (operation.representation != NULL) {
            sprintf(retString, "%s", *operation.representation);
        } else {
            sprintf(retString, "Variable");
        }
        return retString;
    }
    struct Operation inner, left, right;
    char* innerString; char* leftString; char* rightString;
    switch (operation.type) {
        case ABS_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "abs(%s)", innerString);
            free(innerString);
            return retString;
        case ADDITION_OPERATION:
            left = *operation.values[0];
            right = *operation.values[1];
            leftString = toStringOperation(left);
            rightString = toStringOperation(right);
            sprintf(retString, "%s + %s", leftString, rightString);
            free(leftString);
            free(rightString);
            return retString;
        case ARCCOS_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "arccos(%s)", innerString);
            free(innerString);
            return retString;
        case ARCSIN_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "arcsin(%s)", innerString);
            free(innerString);
            return retString;
        case ARCTAN_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "arctan(%s)", innerString);
            free(innerString);
            return retString;
        case COS_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "cos(%s)", innerString);
            free(innerString);
            return retString;
        case DIVISION_OPERATION:
            left = *operation.values[0];
            right = *operation.values[1];
            leftString = toStringOperation(left);
            rightString = toStringOperation(right);
            sprintf(retString, "(%s) / (%s)", leftString, rightString);
            free(leftString);
            free(rightString);
            return retString;
        case LOG_OPERATION:
            left = *operation.values[0];
            right = *operation.values[1];
            leftString = toStringOperation(left);
            rightString = toStringOperation(right);
            sprintf(retString, "log(%s, %s)", leftString, rightString);
            free(leftString);
            free(rightString);
            return retString;
        case MULTIPLICATION_OPERATION:
            left = *operation.values[0];
            right = *operation.values[1];
            leftString = toStringOperation(left);
            rightString = toStringOperation(right);
            sprintf(retString, "(%s) * (%s)", leftString, rightString);
            free(leftString);
            free(rightString);
            return retString;
        case NEGATION_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "-%s", innerString);
            free(innerString);
            return retString;
        case PARENTHESES_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "(%s)", innerString);
            free(innerString);
            return retString;
        case POWER_OPERATION:
            left = *operation.values[0];
            right = *operation.values[1];
            leftString = toStringOperation(left);
            rightString = toStringOperation(right);
            sprintf(retString, "(%s) ^ (%s)", leftString, rightString);
            free(leftString);
            free(rightString);
            return retString;
        case SIN_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "sin(%s)", innerString);
            free(innerString);
            return retString;
        case SUBTRACTION_OPERATION:
            left = *operation.values[0];
            right = *operation.values[1];
            leftString = toStringOperation(left);
            rightString = toStringOperation(right);
            sprintf(retString, "%s - %s", leftString, rightString);
            free(leftString);
            free(rightString);
            return retString;
        case TAN_OPERATION:
            inner = *operation.values[0];
            innerString = toStringOperation(inner);
            sprintf(retString, "tan(%s)", innerString);
            free(innerString);
            return retString;
        default:
            sprintf(retString, "Unknown");
            return retString;
    }
}

int printOperation(struct Operation operation) {
    char* str = toStringOperation(operation);
    printf("%s\n", str);
    return 0;
}

int strEquals(struct Operation one, struct Operation other){
    char* str1 = toStringOperation(one);
    char* str2 = toStringOperation(other);
    int compared = strcmp(str1, str2);
    return compared == 0;
}

double complex toNumber(struct Operation operation) {
    double complex inner;
    double complex left;
    double complex right;
    switch (operation.type) {
        case CONSTANT_OPERATION:
            return *operation.number_representation;
        case VARIABLE_OPERATION:
            return NAN + NAN * I;
        case ABS_OPERATION:
            inner = toNumber(*operation.values[0]);
            return cabs(inner);
        case ADDITION_OPERATION:
            double complex sum = 0 + 0*I;
            for (int i = 0; i < operation.numValues; ++i) {
                sum += toNumber(*(operation.values[i]));
            }
            return sum;
        case ARCCOS_OPERATION:
            inner = toNumber(*operation.values[0]);
            return cacos(inner);
        case ARCSIN_OPERATION:
            inner = toNumber(*operation.values[0]);
            return casin(inner);
        case ARCTAN_OPERATION:
            inner = toNumber(*operation.values[0]);
            return catan(inner);
        case COS_OPERATION:
            inner = toNumber(*operation.values[0]);
            return ccos(inner);
        case DIVISION_OPERATION:
            left = toNumber(*operation.values[0]);
            right = toNumber(*operation.values[1]);
            return left / right;
        case LOG_OPERATION:
            left = toNumber(*operation.values[0]);
            right = toNumber(*operation.values[1]);
            return clog(right) / clog(left);
        case MULTIPLICATION_OPERATION:
            double complex product = 1 + 0*I;
            for (int i = 0; i < operation.numValues; ++i) {
                product *= toNumber(*(operation.values[i]));
            }
            return product;
        case NEGATION_OPERATION:
            inner = toNumber(*operation.values[0]);
            return -1 * inner;
        case PARENTHESES_OPERATION:
            inner = toNumber(*operation.values[0]);
            return inner;
        case POWER_OPERATION:
            left = toNumber(*operation.values[0]);
            right = toNumber(*operation.values[1]);
            return cpow(left, right);
        case SIN_OPERATION:
            inner = toNumber(*operation.values[0]);
            return csin(inner);
        case SUBTRACTION_OPERATION:
            left = toNumber(*operation.values[0]);
            right = toNumber(*operation.values[1]);
            return left - right;
        case TAN_OPERATION:
            inner = toNumber(*operation.values[0]);
            return ctan(inner);
        default:
            fprintf(stderr, "Unsupported operation in toNumber.\n");
            return NAN + NAN * I;
    }
    return NAN + NAN * I;
}


int containsOperation(struct Operation one, int size, struct Operation** array) {
    for (int i = 0; i < size; ++i) {
        struct Operation other = *(array[i]);
        if (equalsOperations(one, other)) {
            return true;
        }
    }
    return false;
}


int containsAllOperations(struct Operation one, struct Operation two) {
    if (one.numValues != two.numValues) {
        return false;
    }
    for (int i = 0; i < one.numValues; ++i) {
        if (containsOperation(one, one.numValues, two.values) == false) {
            return false;
        }
    }
    return true;
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemoryLeak"
int equalsOperations(struct Operation one, struct Operation two) {
    int oneIsConstant = isConstant(one);
    int twoIsConstant = isConstant(two);
    if (oneIsConstant && twoIsConstant) {
        return toNumber(one) == toNumber(two);
    }
    if (oneIsConstant != twoIsConstant) {
        return false;
    }
    if (one.numValues == 0 && two.numValues == 0) {
        return strEquals(one, two);
    }
    struct Operation oneFlattened = deepFlatten(one);
    struct Operation twoFlattened = deepFlatten(two);
    free(oneFlattened.values);
    free(twoFlattened.values);
    if ((one.type == two.type) && (oneFlattened.numValues != twoFlattened.numValues)) {
        return false;
    }
    if ((one.type == two.type) && containsAllOperations(one, two)) {
        return true;
    }
    return strEquals(one, two);
}
#pragma clang diagnostic pop