#ifndef CMACS_BUFFER_H
#define CMACS_BUFFER_H

#include "str_buffer.h"

struct cursor {
    unsigned int line;
    unsigned int column;
};

typedef struct {
    unsigned int line_count;

    StrBuffer *current_line;

    struct cursor cursor;
    
    StrBuffer *head;
    StrBuffer *tail;
} CmacsBuffer;

CmacsBuffer *CmacsBuffer_Create(void);

int CmacsBuffer_AddLine(CmacsBuffer *buffer);

int CmacsBuffer_RemoveLine(CmacsBuffer *buffer);

void CmacsBuffer_Destroy(CmacsBuffer *buffer);

#endif
