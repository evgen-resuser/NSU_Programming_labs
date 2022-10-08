#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

enum Messages{
    SUCCESS = 1,
    ERROR = 0,
};

enum StrSize {
    STNDRT_STRING_SIZE = 256,
};

void swap(char* a, char* b){
    char buffer = *a;
    *a = *b;
    *b = buffer;
}

int permutation(char str[STNDRT_STRING_SIZE], const size_t len) { //using the Narayana's algorithm
    size_t j, l, maxJ = len+2, maxL = 0;

    for (j = 0; j < len; ++j) {
        if (str[j] < str[j + 1]) if (j > maxJ || maxJ == len+2) maxJ = j;
    }
    if (maxJ == len + 2) return ERROR; // if maxJ hasn't changed, then there are no new permutations

    for (l = maxJ + 1; l < len; ++l) {
        if (str[maxJ] < str[l]) if (l > maxL) maxL = l;
    }

    swap(&str[maxL], &str[maxJ]);

    // reversing the part of string
    size_t strt = maxJ + 1;
    size_t k = len - 1; //
    size_t count = (int)((len - strt) / 2);
    for (size_t i = 0; i != count; i++) {
        swap(&str[strt + i], &str[k]);
        k--;
    }
    return SUCCESS;
}

int stringCheck(char const str[STNDRT_STRING_SIZE],const size_t len){
    int arrayOfRepeats[STNDRT_STRING_SIZE] = {0};
    for (size_t i = 0; i < len; ++i)
    {
        if ('0' > str[i] || str[i] > '9')
            return ERROR;
        ++arrayOfRepeats[ (size_t)str[i] ];
        if (arrayOfRepeats[ (size_t)str[i] ] > 1)
            return ERROR;
    }
    return SUCCESS;
}

int main() {
    char str[STNDRT_STRING_SIZE];
    size_t count;
    if (fgets(str, STNDRT_STRING_SIZE, stdin) == NULL){
        printf("bad input");
        return ERROR;
    }

    if (scanf("%zu", &count) == EOF){
        printf("bad input");
        return ERROR;
    }

    size_t length = strlen(str)-1; //
    if (stringCheck(str, length) == ERROR){
        printf("bad input");
        return ERROR;
    }
    for (size_t i = 0; i != count; ++i) { //size_t
        int res = permutation(str, length);
        if (res == SUCCESS) printf("%s", str);
    }
    return 0;
}
