#ifndef CMACS_DEBUG_H
#define CMACS_DEBUG_H

#include <stdio.h>
#include <stdarg.h>

typedef enum {
    INFO,
    WARN,
    ERR
} DEBUG_LEVEL;

int d_printf(DEBUG_LEVEL debug_level, const char * format, ...);

#endif