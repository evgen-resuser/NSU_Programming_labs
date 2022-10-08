#include <stdio.h>
#include <string.h>

#define SUCCESS 0

enum sizes{
    TEST_SIZE = 257,
    PATTERN_SIZE = 18,
};

void getShiftTable(const char* pattern, size_t shiftTable[]){
    const size_t pattrnLen = strlen(pattern);

    for (size_t i = 0; i < TEST_SIZE; ++i){
        shiftTable[i] = pattrnLen;
    }

    for (size_t i = 1; i < pattrnLen; ++i){
        if (shiftTable[(unsigned char)pattern[pattrnLen - 1 - i]] == pattrnLen){
            shiftTable[(unsigned char)pattern[pattrnLen - 1 - i]] = i;
        }
    }
}

void doBoyerMoore (const char* pattern, const size_t patternLen){
    size_t shiftTable[TEST_SIZE] = {0};
    getShiftTable(pattern, shiftTable);
    char text[TEST_SIZE]={0};
    long int textLen=0;
    while (1) {
        if (fgets(text, TEST_SIZE, stdin) == NULL) return;

        //BoyerMoore
        const size_t Len = strlen(text);

        size_t pos = patternLen - 1;

        while (pos < Len) {
            printf("%zu ",textLen + pos + 1 );
            if (pattern[patternLen - 1] != text[pos]) {
                pos += shiftTable[(int)text[pos]];
            } else {
                for (int i = (int)patternLen - 2; i >= 0; i--) {
                    printf("%zu ", textLen + pos - patternLen + i + 2);
                    if (pattern[i] != text[pos - patternLen + i + 1]) {
                        pos += shiftTable[(int) pattern[patternLen - 1]] ;
                        break;
                    } else if (i == 0)
                        pos += patternLen;
                }
            }
        }
        textLen += (int)strlen(text);
        for (size_t i = 0; i != TEST_SIZE; i++) text[i] = '0';
    }
}

int main() {
    char pattern[PATTERN_SIZE];
    if (fgets(pattern, PATTERN_SIZE, stdin) == NULL) return SUCCESS;
    size_t patternLen = strlen(pattern) - 1;
    if (pattern[patternLen] == '\n') pattern[patternLen] = '\0';

    doBoyerMoore(pattern, patternLen);

    return SUCCESS;
}
