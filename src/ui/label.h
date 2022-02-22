#ifndef CMACS_LABEL_H
#define CMACS_LABEL_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct Label_ {
    const char      *text;
    SDL_Texture     *texture;
    SDL_Color        color;
    SDL_Rect         rect;
    SDL_Rect         text_rect;
    int              pad_x;
    int              pad_y;
    bool             background;
    SDL_Texture     *background_texture;
} Label;

Label *Label_Create(SDL_Renderer *renderer, const char *label_text, int x, int y, TTF_Font *label_font, SDL_Color text_color, bool background, int pad_x, int pad_y);

void Label_SetPos(Label *label, int x, int y);

void Label_RenderCopy(SDL_Renderer *renderer, Label *label);

void Label_Destroy(Label *label);

#endif
