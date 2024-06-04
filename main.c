#include "include/cast/operation.h"
#include "tests/test_bulk.h"
#include "tests/test_tokenize.h"
#include "tests/test_util.h"

int main(void) {
    testBulk();
    testUtil();
    testTokenize();
    return 0;
}
