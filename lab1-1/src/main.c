#include <stdio.h>

#define BUFFER_SIZE 256
#define PATTERN_SIZE 16

#define SUCCESS 0

void myStrlen(const unsigned char *str, size_t *len){
    for (*len = 0; str[*len]; (*len)++);
}

size_t inputPattern(unsigned char* pattern){
    size_t i = 0;
    size_t patternLen = 0;
    while(1){
        pattern[i] = getchar();
        if (pattern[i] == '\n'){
            pattern[i] = '\0';
            break;
        }
        i++;
    }
    myStrlen(pattern, &patternLen);
    return patternLen;
}

size_t hashFunction(const unsigned char* pattern, const size_t patternLen){
    int basePow = 1;
    int hash = 0;
    for (size_t i = 0; i < patternLen; i++){
        hash += (pattern[i] % 3) * basePow;
        basePow *= 3;
    }
    return hash;
}

void doRabinKarp(const unsigned char* pattern, const size_t patternLen){
    size_t hashPattern = hashFunction(pattern, patternLen);
    printf("%zu ", hashPattern);

    //find the biggest degree of a pattern
    int hashPow = 1;
    for (size_t i = 0; i < patternLen-1; i++) hashPow *= 3;

    unsigned char buffer[BUFFER_SIZE];
    size_t textLen = 0;

    while(1){
        if (fgets((char*)buffer, BUFFER_SIZE, stdin) == NULL) break;

        size_t buffLen = 0;
        myStrlen(buffer, &buffLen);

        size_t pttrnLenCopy = patternLen; //copy of length of pattern that can be changed

        if (buffLen + 1 > patternLen) {
            size_t textHash = hashFunction(buffer, patternLen);
            while (pttrnLenCopy != buffLen + 1) {
                if (textHash == hashPattern) {
                    size_t pos = pttrnLenCopy - patternLen;
                    while(pos != pttrnLenCopy){
                        printf("%zu ", textLen + pos + 1);
                        
                        if (buffer[pos] != pattern[pos + patternLen - pttrnLenCopy]) break;
                        
                        ++pos;
                    }
                }
                //change the hash without full recalculation
                textHash = ((textHash - (buffer[pttrnLenCopy - patternLen] % 3)) / 3) + ((buffer[pttrnLenCopy] % 3) * hashPow);

                ++pttrnLenCopy;
            }
        }
        textLen += buffLen;
    }

}

int main() {
    unsigned char pattern[PATTERN_SIZE+2] = {0};
    size_t patternLen = inputPattern(pattern);
    doRabinKarp(pattern, patternLen);
    return SUCCESS;
}
