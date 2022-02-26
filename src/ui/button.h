#ifndef CMACS_BUTTON_H
#define CMACS_BUTTON_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct Button_ {
    const char      *text;
    SDL_Color        text_color;
    SDL_Texture     *text_texture;
    SDL_Rect         text_rect;
    
    SDL_Rect         rect;
    int              pad_x;
    int              pad_y;
    SDL_Texture     *background_texture;
    SDL_Texture     *hovered_background_texture;
    SDL_Texture     *pressed_background_texture;

    bool             is_hovered;
    bool             is_pressed;

    struct Button_  *next;
} Button;

Button *Button_Create(
    SDL_Renderer *renderer, 
    int x, int y, 
    const char *button_text, 
    TTF_Font *button_font, 
    SDL_Color text_color, 
    int pad_x, int pad_y
);

void Button_SetPos(Button *button, int x, int y);

void Button_HoverCheck(Button *button, int mouse_x, int mouse_y);

void Button_PressCheck(Button *button);

void Button_ReleaseCheck(Button *button);

void Button_RenderCopy(SDL_Renderer *renderer, Button *button);

void Button_Destroy(Button *button);


// Button register and the functions that use it.
typedef struct ButtonRegister_ {
    Button *top_button;
} ButtonRegister;

extern ButtonRegister button_register;

void Button_CallFor_AllButtons(void (*callback)(Button *btn));

void Button_HoverCheck_AllButtons(int mouse_x, int mouse_y);

void Button_RenderCopy_AllButtons(SDL_Renderer *renderer);

#endif
