#ifndef CMACS_STRBUFFER_H
#define CMACS_STRBUFFER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _StrBuffer {
    unsigned int buff_size;
    unsigned int str_length;
    char *data;

    struct _StrBuffer *next;
    struct _StrBuffer *prev;
} StrBuffer;

StrBuffer *StrBuffer_Create(unsigned int init_length);

// StrBuffer_AppendChar

int StrBuffer_AddChar(StrBuffer *str_buffer, char character, int position);

int StrBuffer_RemoveChar(StrBuffer *str_buffer, int position);

int StrBuffer_CopyAndAppendContents(StrBuffer *source, StrBuffer *dest, int start_position);

int StrBuffer_MoveAndAppendContents(StrBuffer *source, StrBuffer *dest, int start_position);

void StrBuffer_Destroy(StrBuffer *str_buffer);

#endif
