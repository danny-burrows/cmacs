#ifndef CMACS_LABEL_H
#define CMACS_LABEL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct Label_ {
    const char      *text;
    SDL_Texture     *texture;
    SDL_Color        color;
    SDL_Rect         rect;

    SDL_Texture     *background_texture;
    SDL_Rect         background_rect;
} Label;

Label *Label_Create(SDL_Renderer *renderer, const char *label_text, int x, int y, TTF_Font *label_font);

void Label_RenderCopy(SDL_Renderer *renderer, Label *label);

#endif
