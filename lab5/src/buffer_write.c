#include "buffer_write.h"

#include <stdio.h>
#include <stdlib.h>

#define BUFF_LEN 1024
#define CHAR_SIZE 256
#define CHAR_BITS 8

struct WBuffer {
  unsigned char *buffer;
  size_t indx;
  size_t cur;
  FILE* out;
};

WBuffer * initBufW(FILE* out){
  WBuffer *buff = malloc(sizeof(WBuffer));
  if (buff == NULL) return NULL;
  buff->buffer = calloc(BUFF_LEN, sizeof(unsigned char));
  if (buff->buffer == NULL) {
      free(buff);
      return NULL; }

  buff->out = out;
  buff->cur = 0;
  buff->indx = 0;
  return buff;
}

void putChar(WBuffer* const buff, const unsigned char toAdd){
  buff->buffer[buff->cur] = toAdd;
  ++buff->cur;
  if (buff->cur == BUFF_LEN) {
    fwrite(buff->buffer, sizeof(unsigned char), BUFF_LEN, buff->out);
    buff->cur = 0;
  }
}

void flush(WBuffer* const buff){
  if (!buff->cur) return;
  fwrite(buff->buffer, sizeof(unsigned char), buff->cur, buff->out);
  buff->cur = 0;
}

void freeWBuffer(WBuffer * buff){
  free(buff->buffer);
  free(buff);
}

unsigned char* bWbuff(WBuffer* buff){
    return buff->buffer;
}

size_t* bWindx(WBuffer* buff){
    return &buff->indx;
}

void flushByIndx(WBuffer* const buff){
    if (!buff->indx) return;
    unsigned char temp = buff->buffer[buff->indx/CHAR_BITS];
    fwrite(buff->buffer, sizeof(unsigned char), buff->indx/CHAR_BITS, buff->out);

    buff->buffer[0] = temp;
    for (size_t i = 1; i < (buff->indx)/CHAR_BITS+1; ++i)
        buff->buffer[i] = 0;
    buff->indx = buff->indx % CHAR_BITS;
}

void addBit(WBuffer* const buff, const char toAdd){
    if (toAdd == '1') buff->buffer[buff->indx/8] |= (1<<(7-(buff->indx%8)));
    ++(buff->indx);
}

void addSymbolToBuff(WBuffer* const buff, const unsigned char sym){
    if (buff->indx % CHAR_BITS == 0){
        buff->buffer[buff->indx / CHAR_BITS] = sym;
        buff->indx = buff->indx + CHAR_BITS;
    } else {
        size_t toAdd = buff->indx % CHAR_BITS;
        unsigned char part = sym >> toAdd;
        buff->buffer[buff->indx / CHAR_BITS] = buff->buffer[buff->indx / CHAR_BITS] | part;
        part = sym << (CHAR_BITS-toAdd);
        buff->buffer[(buff->indx / CHAR_BITS)+1] = buff->buffer[buff->indx / CHAR_BITS+1] | part;
        buff->indx = buff->indx + CHAR_BITS;
    }
}

char readFirstBit(WBuffer* const buff, const size_t num){
    char first = '0';
    if (((buff->buffer[num]) & (1<<(CHAR_BITS-1)))) first = '1';
    if (buff->indx == 0) first = '\0';
    return first;
}

void moveLine(WBuffer* const buff){
    for (size_t i = 0; i < (buff->indx)/CHAR_BITS+1; i++){
        char first;
        buff->buffer[i] = buff->buffer[i]<<1;
        first = readFirstBit(buff, i+1);
        if (first == '1') buff->buffer[i] |= 1;
    }
    buff->indx--;
}

