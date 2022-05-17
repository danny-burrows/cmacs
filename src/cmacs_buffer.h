#ifndef CMACS_BUFFER_H
#define CMACS_BUFFER_H

#include "str_buffer.h"

typedef struct {
    unsigned int line_count;

    StrBuffer *current_line;
    
    StrBuffer *head;
    StrBuffer *tail;
} CmacsBuffer;

CmacsBuffer *CmacsBuffer_Create(void);

int CmacsBuffer_InsertLine(CmacsBuffer *buffer);

int CmacsBuffer_RemoveLine(CmacsBuffer *buffer);

void CmacsBuffer_Destroy(CmacsBuffer *buffer);

#endif
