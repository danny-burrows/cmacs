#include "str_buffer.h"

StrBuffer *StrBuffer_Create(unsigned int init_size) {

    // Two callocs is annoying :/
    StrBuffer *buffer = (StrBuffer *)calloc(1, sizeof(StrBuffer));
    buffer->data = (char *)calloc(1, sizeof(char) * init_size);
    
    buffer->buff_size = init_size;
    return buffer;
}

int StrBuffer_AddChar(StrBuffer *str_buffer, char character, int position) {

    // Double buffsize if needed. I.e. if the next char plus '\0' is over buf_size.
    if (str_buffer->str_length + 2 > str_buffer->buff_size) {
        str_buffer->buff_size *= 2;
        str_buffer->data = realloc(str_buffer->data, sizeof(char) * str_buffer->buff_size);
        memset(&str_buffer->data[str_buffer->str_length], 0, str_buffer->buff_size - str_buffer->str_length);
    }
    str_buffer->str_length++;

    char *c = &str_buffer->data[position];
    char current = character;
    char next = str_buffer->data[position];

    // Pointers faster than [].
    for (unsigned int i = position; i < str_buffer->str_length - 1; i++) {
        *c = current;
        current = next;
        next = *(c + 1);
        c++;
    }
    *c = current;

    return 0;
}

int StrBuffer_RemoveChar(StrBuffer *str_buffer, int position) {
    if (str_buffer->str_length == 0) return -1;

    char *c = &str_buffer->data[position];
    for (unsigned int i = position; i < str_buffer->str_length; i++) {
        *c = *(c + 1);
        c++;
    }

    str_buffer->str_length--;
    return 0;
}

// Returns number of bytes copied.
int StrBuffer_CopyAndAppendContents(StrBuffer *source, StrBuffer *dest, int source_start_position) {

    // NOTE: Needs some error checking.

    if (source->str_length == 0) return 0;

    char *c = &source->data[source_start_position];
    for (unsigned int i = source_start_position; i < source->str_length; i++) {
        StrBuffer_AddChar(dest, *c, dest->str_length);
        c++;
    }
    int r = source->str_length - source_start_position;
    return r;
}

// Returns number of bytes Moved.
int StrBuffer_MoveAndAppendContents(StrBuffer *source, StrBuffer *dest, int source_start_position) {
    
    // NOTE: Needs some error checking.

    if (source->str_length == 0) return 0;

    char *c = &source->data[source_start_position];
    for (unsigned int i = source_start_position; i < source->str_length; i++) {
        StrBuffer_AddChar(dest, *c, dest->str_length);

        // Cleaning up for move...
        *c = '\0';
        
        c++;
    }
    int r = source->str_length - source_start_position;
    source->str_length = source_start_position;
    return r;
}

void StrBuffer_Destroy(StrBuffer *str_buffer) {
    free(str_buffer->data);
    free(str_buffer);
}