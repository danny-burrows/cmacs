#ifndef CMACS_LABEL_H
#define CMACS_LABEL_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct Label_ {
    const char      *text;
    SDL_Color        text_color;
    SDL_Texture     *text_texture;
    SDL_Rect         text_rect;

    SDL_Rect         rect;
    int              pad_x;
    int              pad_y;
    bool             background;
    SDL_Texture     *background_texture;

    struct Label_   *next;
} Label;

Label *Label_Create(
    SDL_Renderer *renderer, 
    int x, int y, 
    const char *label_text, 
    TTF_Font *label_font, 
    SDL_Color text_color, 
    bool background, 
    int pad_x, int pad_y
);

void Label_SetPos(Label *label, int x, int y);

void Label_RenderCopy(SDL_Renderer *renderer, Label *label);

void Label_Destroy(Label *label);

// Label register and the functions that use it.
typedef struct LabelRegister_ {
    Label *top_label;
} LabelRegister;

extern LabelRegister label_register;

void Label_CallFor_AllLabels(void (*callback)(Label *lbl));

void Label_RenderCopy_AllLabels(SDL_Renderer *renderer);

#endif
