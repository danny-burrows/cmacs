#include "cmacs_buffer.h"

CmacsBuffer *CmacsBuffer_Create(void)
{
    CmacsBuffer *buffer = (CmacsBuffer *)calloc(1, sizeof(CmacsBuffer));

    buffer->current_line = StrBuffer_Create(32);
    buffer->head = buffer->current_line;
    buffer->tail = buffer->current_line;
    buffer->line_count = 1;
    
    return buffer;
}

int CmacsBuffer_InsertLine(CmacsBuffer *buffer)
{
    StrBuffer *new_line = StrBuffer_Create(32);
    if (!new_line) return -1;

    new_line->prev = buffer->current_line;
    new_line->next = buffer->current_line->next;
    
    buffer->current_line->next = new_line;
    buffer->current_line = new_line;
    
    if (!buffer->current_line->next) {
        buffer->tail = buffer->current_line;
    } else {
        buffer->current_line->next->prev = buffer->current_line;
    }

    buffer->line_count++;
    return 0;
}

int CmacsBuffer_RemoveLine(CmacsBuffer *buffer)
{
    if (!buffer->current_line->prev) return -1;

    buffer->current_line->prev->next = buffer->current_line->next;

    if (buffer->current_line->next)
        buffer->current_line->next->prev = buffer->current_line->prev;

    StrBuffer *temp = buffer->current_line->prev;
    StrBuffer_Destroy(buffer->current_line);
    buffer->current_line = temp;

    buffer->line_count--;
    return 0;
}

void CmacsBuffer_Destroy(CmacsBuffer *buffer)
{
    buffer->current_line = buffer->head;
    while (buffer->current_line) {
        StrBuffer *temp = buffer->current_line->next;
        StrBuffer_Destroy(buffer->current_line);
        buffer->current_line = temp;
    }
    free(buffer);
}
