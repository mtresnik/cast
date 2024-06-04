#include <stdbool.h>
#include <complex.h>

#ifndef CAST_OPERATION_H
#define CAST_OPERATION_H
#define NULL_OP (-1)
#define CONSTANT_OPERATION (0)
#define VARIABLE_OPERATION (1)
#define ABS_OPERATION (2)
#define ADDITION_OPERATION (3)
#define ARCCOS_OPERATION (4)
#define ARCSIN_OPERATION (5)
#define ARCTAN_OPERATION (6)
#define COS_OPERATION (7)
#define DIVISION_OPERATION (8)
#define LOG_OPERATION (9)
#define MULTIPLICATION_OPERATION (10)
#define NEGATION_OPERATION (11)
#define PARENTHESES_OPERATION (12)
#define POWER_OPERATION (13)
#define SIN_OPERATION (14)
#define SUBTRACTION_OPERATION (15)
#define TAN_OPERATION (16)

struct Operation {
    int numValues;
    struct Operation **values;
    int type;
    char** representation;
    double complex* number_representation;
};

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

//struct Operation evaluate(struct Operation base, struct Operation one, struct Operation other);
//
int equalsOperations(struct Operation one, struct Operation two);

#endif //CAST_OPERATION_H
