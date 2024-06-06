#ifndef CAST_TOKEN_H
#define CAST_TOKEN_H

#define NUMBER_TOKEN (0)
#define OPERATOR_TOKEN (1)
#define OPEN_PARENTHESIS_TOKEN (2)
#define CLOSE_PARENTHESIS_TOKEN (3)
#define TEXT_TOKEN (4)
#define FUNCTION_TOKEN (5)
#define VARIABLE_TOKEN (6)

struct Token {
    int startIndex;
    int endIndex;
    int type;
    char* representation;
};

struct TokenArray {
    int numValues;
    int arraySize;
    struct Token* values;
};

size_t sizeOfToken(struct Token token);

size_t sizeOfTokenArray(struct TokenArray array);

struct TokenArray nullTokenArray();

struct TokenArray singleTokenArray(struct Token token);

struct TokenArray doubleTokenArray(struct Token left, struct Token right);

struct Token cloneToken(struct Token other);

struct TokenArray cloneTokenArray(struct TokenArray base);

struct TokenArray appendTokenArray(struct TokenArray base, struct Token token);

struct TokenArray concatTokenArrays(struct TokenArray base, struct TokenArray other);

struct Token singleIndex(int index, int type);

struct Token nullIndex(int type, char* representation);

struct Token convert(struct Token other, int type);

char* toStringToken(struct Token t);

int printToken(struct Token t);

int printTokens(struct TokenArray t);

int equalsToken(struct Token one, struct Token other);

int containsToken(struct TokenArray oneArray, struct Token other);

int containsAllTokens(struct TokenArray oneArray, struct TokenArray otherArray);

#endif //CAST_TOKEN_H
