#include <stdio.h>
#include "../include/cast/operation.h"

struct Operation createComplexConstantAndPrint(double real, double imag) {
    double complex complexNumber = real + imag * I;
    struct Operation constantOperation = Constant(complexNumber);
    printOperation(constantOperation);
    return constantOperation;
}

int testBulkComplexNumbersOperation() {
    char *operationName = "testBulkComplexNumbersOperation";
    printf("---------------%s--------------\n", operationName);

    struct Operation constant1 = createComplexConstantAndPrint(2, 2);
    struct Operation constant2 = createComplexConstantAndPrint(3, 1);
    struct Operation constant3 = createComplexConstantAndPrint(-5, 0);

    struct Operation addition1 = Addition(&constant1, &constant2);
    printOperation(addition1);

    struct Operation absOperation1 = Abs(&constant3);
    printOperation(absOperation1);

    struct Operation variableOperation = Variable("x");
    printOperation(variableOperation);

    struct Operation addition2 = Addition(&addition1, &absOperation1);
    struct Operation addition3 = Addition(&addition2, &variableOperation);

    struct Operation absOperation2 = Abs(&variableOperation);
    printOperation(absOperation2);

    struct Operation finalAddition = Addition(&addition3, &absOperation2);
    printOperation(finalAddition);

    freeOperation(constant1);
    freeOperation(constant2);
    freeOperation(constant3);
    freeOperation(addition1);
    freeOperation(absOperation1);
    freeOperation(variableOperation);
    freeOperation(addition2);
    freeOperation(addition3);
    freeOperation(absOperation2);
    freeOperation(finalAddition);

    return 0;
}

int testBulkNestedAddition() {
    char* testName = "testBulkNestedAddition";
    printf("---------------%s--------------\n", testName);
    double complex number = 2 + 2 * I;
    struct Operation constant1 = Constant(number);

    struct Operation variable1 = Variable("x");
    struct Operation variable2 = Variable("y");

    struct Operation addition1 = Addition(&variable1, &variable2);

    struct Operation addition2 = Addition(&constant1, &addition1);
    printOperation(addition2);
    return  0;
}

int testBulkMultiplication() {
    char* testName = "testBulkMultiplication";
    printf("---------------%s--------------\n", testName);
    double complex number = 2 + 2 * I;
    struct Operation constant1 = Constant(number);

    double complex number2 = 2;
    struct Operation constant2 = Constant(number2);
    printOperation(constant1);
    printOperation(constant2);

    struct Operation multiplication = Multiplication(&constant1, &constant2);
    printOperation(multiplication);

    freeOperation(constant1);
    freeOperation(constant2);
    freeOperation(multiplication);

    return 0;
}

int testBulkDivision() {
    char *testName = "testBulkDivision";
    printf("---------------%s--------------\n", testName);
    double complex number1 = 4 + 4 * I;
    struct Operation constant1 = Constant(number1);
    double complex number2 = 2;
    struct Operation constant2 = Constant(number2);
    printOperation(constant1);
    printOperation(constant2);
    struct Operation division = Division(&constant1, &constant2);
    printOperation(division);
    return 0;
}

int testBulk(void) {
    testBulkComplexNumbersOperation();
    testBulkNestedAddition();
    testBulkMultiplication();
    testBulkDivision();
    return 0;
}


