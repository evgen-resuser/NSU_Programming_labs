#ifndef LAB5_BUFFER_READ_H
#define LAB5_BUFFER_READ_H

#include <stdio.h>

typedef struct RBuffer RBuffer;

RBuffer * initBufR(FILE*in);
int getChar(RBuffer * buff, unsigned char* sym);
void freeRBuffer(RBuffer * buff);


#endif // LAB5_BUFFER_READ_H
