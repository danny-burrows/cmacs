#ifndef CMACS_FONTS_H
#define CMACS_FONTS_H

#include <SDL2/SDL_ttf.h>

typedef struct Fonts_ {
    TTF_Font *font_regular_bold;
    TTF_Font *font_regular;
    TTF_Font *font_large;
    TTF_Font *font_larger;
} Fonts;

extern Fonts fonts;

int load_fonts(void);

#endif
