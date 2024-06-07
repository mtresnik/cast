# cast
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)]()
[![version](https://img.shields.io/badge/version-1.0.0-blue)]()
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-green.svg?style=flat-square)](https://makeapullrequest.com)
<hr>
Cast is an abstract syntax tree library written in C. This allows for converting char* variables to mathematical structures.

### Sample Code
```c
#include "include/cast/parser.h"

int main(void) {
    char* toParse = "a * bc + 123 / sin(3.1415 * n) ^ log_(2, 8) - e";
    struct Operation operation = parseOperation(toParse);
    printOperation(operation);
    
    // Replace variables!
    char* aString = "a";
    struct Operation variable = Variable(&aString);
    double complex number = 2 + 2 * I;
    struct Operation constant = Constant(&number);
    struct Operation evaluated = evaluate(operation, variable, constant);
    
    // Or replace entire functions:
    struct Operation toReplace = parseOperation("sin(3.1415 * n)");
    struct Operation replaceWith = parseOperation("2 + 5i");
    struct Operation replaced = evaluate(evaluated, toReplace, replaceWith);
}
```

Soon there will be support for derivatives of functions with respect to some variable.