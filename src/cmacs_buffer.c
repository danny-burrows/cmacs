#include "cmacs_buffer.h"

CmacsBuffer *CmacsBuffer_Create(void)
{
    CmacsBuffer *buffer = (CmacsBuffer *)calloc(1, sizeof(CmacsBuffer));

    buffer->current_line = StrBuffer_Create(32);
    buffer->head = buffer->current_line;
    buffer->tail = buffer->current_line;
    buffer->line_count = 1;
    
    buffer->cursor.line = 0;
    buffer->cursor.column = 0;
    
    return buffer;
}

int CmacsBuffer_AddLine(CmacsBuffer *buffer)
{
    // Add null and < 0 checks.

    StrBuffer *new_line = StrBuffer_Create(32);

    new_line->prev = buffer->current_line;

    if (buffer->current_line) {

        if (!buffer->current_line->next) {
            buffer->tail = new_line;
        }

        new_line->next = buffer->current_line->next;
        buffer->current_line->next = new_line;
    } else {
        buffer->head = new_line;
        buffer->tail = new_line;
    }

    buffer->current_line = new_line;
    buffer->cursor.line++;
    buffer->line_count++;
    
    return 0;
}

int CmacsBuffer_RemoveLine(CmacsBuffer *buffer)
{
    // Add null and < 0 checks.

    if (buffer->current_line->prev)
        buffer->current_line->prev->next = buffer->current_line->next;

    if (buffer->current_line->next)
        buffer->current_line->next->prev = buffer->current_line->prev;

    buffer->current_line = buffer->current_line->prev;

    buffer->cursor.line--;
    buffer->line_count--;

    return 0;
}

int CmacsBuffer_UpLine(CmacsBuffer *buffer)
{
    if (buffer->current_line && buffer->current_line->prev) {
        buffer->current_line = buffer->current_line->prev;
        buffer->cursor.line--;
    }

    buffer->cursor.column = buffer->current_line->str_length; // Improvments needed. 
    return 0;                          
}

int CmacsBuffer_DownLine(CmacsBuffer *buffer)
{
    if (buffer->current_line && buffer->current_line->next) {
        buffer->current_line = buffer->current_line->next;
        buffer->cursor.line++;
    }

    buffer->cursor.column = buffer->current_line->str_length; // Improments needed.                 
    return 0;
}

void CmacsBuffer_Destroy(CmacsBuffer *buffer)
{
    // NEED TO: While through and free all lines!

    free(buffer);
}