#include "textures.h"

Textures textures;

int load_textures(SDL_Renderer *renderer)
{    
    SDL_Surface *label_background_surface = IMG_Load("assets/ui/label-background.svg");
    textures.label_background = SDL_CreateTextureFromSurface(renderer, label_background_surface);
    SDL_FreeSurface(label_background_surface);

    SDL_Surface *button_hovered_background_surface = IMG_Load("assets/ui/button-hovered-background.svg");
    textures.button_hovered_background = SDL_CreateTextureFromSurface(renderer, button_hovered_background_surface);
    SDL_FreeSurface(button_hovered_background_surface);

    SDL_Surface *button_pressed_background_surface = IMG_Load("assets/ui/button-pressed-background.svg");
    textures.button_pressed_background = SDL_CreateTextureFromSurface(renderer, button_pressed_background_surface);
    SDL_FreeSurface(button_pressed_background_surface);
    
    return 0;
}
