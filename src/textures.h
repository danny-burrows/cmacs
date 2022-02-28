#ifndef CMACS_TEXTURES_H
#define CMACS_TEXTURES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Textures_ {
    SDL_Texture *label_background;
    SDL_Texture *button_hovered_background;
    SDL_Texture *button_pressed_background;
    SDL_Texture *min_icon;
    SDL_Texture *max_icon;
    SDL_Texture *exit_icon;
    SDL_Texture *red_label_background;
} Textures;

extern Textures textures;

int load_textures(SDL_Renderer *renderer);

#endif
