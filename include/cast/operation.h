#include <stdbool.h>
#include <complex.h>

#ifndef CAST_OPERATION_H
#define CAST_OPERATION_H

struct Operation {
    int numValues;
    struct Operation **values;
    int type;
    char** representation;
    double complex* number_representation;
};

struct OperationArray {
    int numValues;
    struct Operation **values;
};

// nullOperation is used as a sentinel value, for stubbed functions.
struct Operation nullOperation(int numValues, struct Operation **values);

struct Operation NamedConstant(double complex* number, char* name);

struct Operation Constant(double complex* number_representation);

struct Operation Variable(char** representation);

struct Operation Abs(struct Operation *one);

struct Operation Addition(struct Operation *one, struct Operation *other);

struct Operation ArcCos(struct Operation *one);

struct Operation ArcSin(struct Operation *one);

struct Operation ArcTan(struct Operation *one);

struct Operation Cos(struct Operation *one);

struct Operation Division(struct Operation *one, struct Operation *other);

struct Operation Log(struct Operation *one, struct Operation *other);

struct Operation Multiplication(struct Operation *one, struct Operation *other);

struct Operation Negation(struct Operation *one);

struct Operation Parentheses(struct Operation *one);

struct Operation Power(struct Operation *one, struct Operation *other);

struct Operation Sin(struct Operation *one);

struct Operation Subtraction(struct Operation *one, struct Operation *other);

struct Operation Tan(struct Operation *one);

int isConstant(struct Operation operation);

char* toStringOperation(struct Operation operation);

int printOperation(struct Operation operation);

double complex toNumber(struct Operation operation);

struct Operation evaluate(struct Operation base, struct Operation one, struct Operation other);

int equalsOperations(struct Operation one, struct Operation two);

#endif //CAST_OPERATION_H
