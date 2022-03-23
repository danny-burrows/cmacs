#ifndef CMACS_WINDOW_H
#define CMACS_WINDOW_H

#include <SDL2/SDL.h>

#include "fonts.h"
#include "textures.h"
#include "cmacs_buffer.h"

#define MAX_LINES 15000

struct cursor {
    unsigned int line;
    unsigned int column;
};

typedef struct {
    CmacsBuffer *buffer;
    struct cursor cursor; 
} Window;

Window *Window_Create(void);

int Window_NewLine(Window *window);

int Window_RemoveLine(Window *window);


int Window_CursorUp(Window *buffer);

int Window_CursorDown(Window *buffer);

int Window_CursorLeft(Window *buffer);

int Window_CursorRight(Window *buffer);


int Window_Render(Window *window, SDL_Renderer *renderer);

void Window_Destroy(Window *window);

#endif
