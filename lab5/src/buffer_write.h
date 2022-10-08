#ifndef LAB5_BUFFER_WRITE_H
#define LAB5_BUFFER_WRITE_H

#include <stdio.h>

typedef struct WBuffer WBuffer;

WBuffer * initBufW(FILE* out);
void putChar(WBuffer * buff, unsigned char toAdd);
void flush(WBuffer * buff);
void freeWBuffer(WBuffer * buff);

unsigned char* bWbuff(WBuffer* buff);
size_t* bWindx(WBuffer* buff);
void flushByIndx(WBuffer* buff);

void addBit(WBuffer* buff, char toAdd);
void addSymbolToBuff(WBuffer* buff, unsigned char sym);
char readFirstBit(WBuffer* buff, size_t num);
void moveLine(WBuffer* buff);

#endif // LAB5_BUFFER_WRITE_H
