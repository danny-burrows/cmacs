#ifndef CMACS_STRBUFFER_H
#define CMACS_STRBUFFER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    unsigned int buff_size;
    unsigned int str_length;
    char *data;
} StrBuffer;

StrBuffer *StrBuffer_Create(unsigned int init_length);

int StrBuffer_AddChar(StrBuffer *str_buffer, char character, int position);

int StrBuffer_RemoveChar(StrBuffer *str_buffer, int position);

void StrBuffer_Destroy(StrBuffer *str_buffer);

#endif
