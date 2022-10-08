#include "buffer_read.h"

#include <stdio.h>
#include <stdlib.h>

#define BUFF_LEN 1024

struct RBuffer {
  unsigned char *buffer;
  size_t cur;
  size_t len;
  FILE* in;
};

RBuffer * initBufR(FILE*in){
  RBuffer *buff = malloc(sizeof(RBuffer));
  if (buff == NULL) return NULL;
  buff->buffer = malloc(BUFF_LEN * sizeof(unsigned char));
  if (buff->buffer == NULL) {
      free(buff);
      return NULL; }

  buff->in = in;
  buff->cur = 0;
  buff->len = 0;
  return buff;
}

int getChar(RBuffer* const buff, unsigned char* sym){
  if (buff->cur == buff->len) {
    buff->len = fread(buff->buffer, sizeof(unsigned char), BUFF_LEN, buff->in);
    if (!buff->len) return 0;
    buff->cur = 0;
  }
  *sym = buff->buffer[buff->cur++];
  return 1;
}

void freeRBuffer(RBuffer * buff){
  free(buff->buffer);
  free(buff);
}

